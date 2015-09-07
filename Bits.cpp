#include "Bits.h"
using namespace std;

/**
 * Create a new object of Bits. 'data' will be set to NULL.
 * 'position' and 'max_position' will be set to 0.
 */
Bits::Bits(){
	this->data = NULL;
	this->position = 0;
	this->max_position = 0;
	this->hash = NULL;
	this->error = false;
}

/**
 * Destroying the Bits object will call 'unload()'.
 */
Bits::~Bits(){
	this->unload();
}

/**
 * Check if the cursor can go back more than N bytes.
 * @param n Bytes to move backwards
 * @return True if possible, false otherwise.
 */
bool Bits::canMoveBackwards(uint64_t n){
	this->unsetError();
	if(this->position == 0 || this->position < n){
		return false;
	}

	return (this->position + n) >= 0;
}

/**
 * Check if the cursor can go forward more than N bytes.
 * @param n Bytes to move forward.
 * @return True if possible, false otherwise.
 */
bool Bits::canMoveForward(uint64_t n){
	this->unsetError();
	return (this->position + n) <= this->max_position;
}

/**
 * Check if the previous call to a method in this object failed.
 * Note that calling this method multiple times won't affect the error state.
 * @return
 */
bool Bits::checkIfError(){
	return this->error;
}

/**
 * Read data from file.
 * @param fname The path/name of the file.
 * @param mode Optional, open mode. Default is "rb".
 * @return True if data was successfully readen, otherwise false.
 */
bool Bits::fromFile(char *fname, ios_base::openmode mode){
	this->unsetError();
	bool state = false;
	streampos size;
	ifstream file;

	if(fname == NULL){
		this->setError();
		goto err;
	}

	file.open(fname, mode);
	if(file.is_open()){
		file.seekg(0, ios::end);
		size = file.tellg();

		if(size == -1){
			this->setError();
			goto err;
		}

		this->data = (unsigned char *) malloc(size);
		if(this->data == NULL){
			this->setError();
			goto err;
		}

		this->position = 0;
		this->max_position = size;
		this->is_from_file = true;

		file.seekg(0, ios::beg);
		file.read((char *) data, size);
		file.close();

		state = true;
	}

	err:

	return state;
}

/**
 * Write data to file. The cursor won't be moved.
 * @param fname The path/name of the file.
 * @param offset An offset from which should start the dumping. Default is 0.
 * @param size The size (in bytes) of the data to be written. Default is the length of the data holded by the object, if offset is 0, otherwise it will be calculated if not passed.
 * @param mode Optional, open mode. Default is "wb".
 * @return True if data was successfully written, otherwise false.
 */
bool Bits::toFile(char *fname, uint64_t offset, uint64_t size, ios_base::openmode mode){
	this->unsetError();
	bool state = false;
	ofstream file;

	if(fname == NULL){
		this->setError();
		goto err;
	}

	if(offset > this->max_position || offset < 0){
		this->setError();
		goto err;
	}

	if(offset == 0 && size == -1){
		size = this->max_position;
	}else if(offset > 0 && size == -1){
		size = this->max_position - offset;
	}

	if(this->max_position < offset + size){
		this->setError();
		goto err;
	}

	file.open(fname, mode);
	if(file.is_open()){
		file.seekp(0, ios::beg);
		file.write((const char *) this->data + offset, size);
		file.close();
		state = true;
	}else{
		goto err;
	}

	err:

	return state;
}

/**
 * Load a memory chunk of data, the size of N bytes.
 * @param chunk A pointer to the data to be loaded.
 * @param size Number of bytes to load.
 * @return True on success, false otherwise.
 */
bool Bits::fromMem(unsigned char *chunk, uint64_t size){
	this->unsetError();
	if(chunk == NULL){
		this->setError();
		return false;
	}else{
		this->data = chunk;
		this->position = 0;
		this->max_position = size;
		this->is_from_file = false;
		return true;
	}
}

/**
 * Unload whatever was loaded (if anything was loaded at all).
 * That means that 'data' will be set to NULL  and free-ed if possible*.
 * Also, 'position' and 'max_position' will be set to 0.
 *
 * If possible means that the data will be free-ed if it was malloc-ed in the
 * first place (if the data was loaded from a file). If the data was loaded
 * from a chunk of memory, it won't be free-ed, so take care of it!
 */
void Bits::unload(){
	this->unsetError();
	if(this->is_from_file){
		free(this->data);
	}

	this->data = NULL;
	this->position = 0;
	this->max_position = 0;
	this->is_from_file = false;

	if(this->hash != NULL){
		free(this->hash);
		this->hash = NULL;
	}
}

/**
 * Read N bytes starting from the current position of the data.
 * The current position will be changed to "current position" + n.
 * @param n Number of bytes to read.
 * @param reverse If set to true (default is false) the data will be returned byte-reversed.
 * @return The data that was readen or NULL if something failed.
 */
unsigned char *Bits::read(uint64_t n, bool reverse){
	this->unsetError();
	if(!this->canMoveForward(n)){
		this->setError();
		return NULL;
	}

	unsigned char *tmp = (unsigned char *) malloc(n);

	if(!tmp){
		this->setError();
		return NULL;
	}

	if(reverse){
		for(uint64_t i = n; i > 0; i--){
			tmp[n - i] = this->data[this->position + i - 1];
		}
		this->position += n;
	}else{
		for(uint64_t i = 0; i < n; i++){
			tmp[i] = this->data[this->position];
			this->position++;
		}
	}

	return tmp;
}

/**
 * Read 8 bits into an uint8_t
 * @return A uint8_t filled with 1 byte of data or NULL if something failed. Cursor is moved 1 position.
 */
uint8_t Bits::read_uint8(){
	this->unsetError();
	unsigned char *c = this->read(1);
	uint8_t i = 0;

	if(c != NULL){
		i = (uint8_t) c[0];
	}else{
		this->setError();
	}

	return i;
}

/**
 * Read 16 bits into an uint16_t
 * @param reverse Read the data starting from the last byte.
 * @return A uint16_t filled with 2 bytes of data or NULL if something failed. Cursor is moved 2 positions forward (or backward if reverse is true).
 */
uint16_t Bits::read_uint16(bool reverse){
	this->unsetError();
	unsigned char *c = this->read(2, reverse);
	uint16_t i = 0;

	if(c != NULL){
		i = (uint16_t) c[1] | ((uint16_t) c[0] << 8);
	}else{
		this->setError();
	}

	return i;
}

/**
 * Read 32 bits into an uint32_t
 * @param reverse Read the data starting from the last byte.
 * @return A uint32_t filled with 4 bytes of data or NULL if something failed. Cursor is moved 4 positions forward (or backward if reverse is true).
 */
uint32_t Bits::read_uint32(bool reverse){
	this->unsetError();
	unsigned char *c = this->read(4, reverse);
	uint32_t i = 0;

	if(c != NULL){
		i = (uint32_t) c[3] | ((uint32_t) c[2] << 8) | ((uint32_t) c[1] << 16) | ((uint32_t) c[0] << 24);
	}else{
		this->setError();
	}

	return i;
}

/**
 * Read 64 bits into an uint64_t
 * @param reverse Read the data starting from the last byte.
 * @return A uint64_t filled with 8 bytes of data or NULL if something failed. Cursor is moved 8 positions forward (or backward if reverse is true).
 */
uint64_t Bits::read_uint64(bool reverse){
	this->unsetError();
	unsigned char *c = this->read(8, reverse);
	uint64_t i = 0;

	if(c != NULL){
		i = (uint64_t) c[7] | ((uint64_t) c[6] << 8) | ((uint64_t) c[5] << 16) | ((uint64_t) c[4] << 24) | ((uint64_t) c[3] << 32) | ((uint64_t) c[2] << 40) | ((uint64_t) c[1] << 48) | ((uint64_t) c[0] << 56);
	}else{
		this->setError();
	}
	return i;
}

/**
 * Read N bytes starting from the current position of the data, without changing the position.
 * @param n Number of bytes to read.
 * @param reverse If set to true (default is false) the data will be returned byte-reversed.
 * @return The data that was readen or NULL if something failed.
 */
unsigned char *Bits::peek(uint64_t n, bool reverse){
	this->unsetError();
	if(!this->canMoveForward(n)){
		this->setError();
		return NULL;
	}

	uint64_t tmppos = this->position;
	unsigned char *tmp = this->read(n, reverse);
	this->position = tmppos;

	return tmp;
}

/**
 * Write N bytes of data read from chunk, starting from the current position of the data.
 * The current position will be changed to "current position" + n.
 * @param chunk The data we want to write.
 * @param n Number of bytes we want to write from the chunk.
 * @param patch If set to false (default is true) data will be inserted without replacing, instead of patching.
 * @return True on success, false otherwise.
 */
bool Bits::write(unsigned char *chunk, uint64_t n, bool patch){
	this->unsetError();
	if(patch && this->canMoveForward(n)){

		memcpy(this->data + this->position, chunk, n);

	}else if(!patch){

		unsigned char *newdata = (unsigned char *) malloc(this->max_position + n);

		//Copy from position 0 to current position
		memcpy(newdata, this->data, this->position);

		//Insert new data
		memcpy(newdata + this->position, chunk, n);

		//Finish copying until the end
		memcpy(newdata + this->position + n, this->data + this->position, this->max_position - this->position);

		if(this->is_from_file){
			free(this->data);
		}

		this->data = newdata;
		this->max_position += n;

	}else{

		this->setError();
		return false;

	}

	this->position += n;
	return true;
}

/**
 * Seek N bytes in the currently loaded data.
 * If `reverse` is true, the position will be decreased.
 * If the operation exceeds the minimum or maximum position, false will be returned.
 * @param n Number of bytes to seek.
 * @param reverse Boolean value for the direction. True will seek backwards.
 * @return True is the operation was successful, false otherwise.
 */
bool Bits::seek(uint64_t n, bool reverse){
	this->unsetError();
	if(n == 0) return true;

	if(!reverse && this->canMoveForward(n)){
		this->position += n;
	}else if(reverse && this->canMoveBackwards(n)){
		this->position -= n;
	}else{
		this->setError();
	}

	return !this->error;
}

/**
 * Find the nearest match to a given pattern in the data behind the current position.
 * The current position will remain unchanged.
 * @param pattern The byte(s) pattern we are looking for.
 * @param n The number of bytes the pattern is long.
 * @return Position/offset or error is set to true and 0 is returned if nothing was found.
 */
uint64_t Bits::findPrevious(unsigned char *pattern, uint64_t n){
	this->unsetError();
	uint64_t res = 0, tmppos = this->position;
	bool found = false;

	while(this->canMoveBackwards()){;
		if(!this->canMoveForward(n)){
			this->seek(1, true);
			continue;
		}

		bool c1 = memcmp(this->data + this->position, pattern, n);
		if(c1 == 0){
			res = this->position;
			found = true;
			break;
		}else{
			this->seek(1, true);
		}
	}

	this->position = tmppos;
	if(found == false){
		this->setError();
	}
	return res;
}

/**
 * Find the nearest match to a given pattern in the data starting at the current position.
 * This, effectively, means that if you search something which is already right next
 * to the current position you'll keep "finding" it forever and ever. Just use "seek" or
 * "setPosition" to go to "current position" + 1 and continue searching from there.
 * The current position will remain unchanged.
 * @param pattern The byte(s) pattern we are looking for.
 * @param n The number of bytes the pattern is long.
 * @return Position/offset or error is set to true and 0 is returned if nothing was found.
 */
uint64_t Bits::findNext(unsigned char *pattern, uint64_t n){
	this->unsetError();
	uint64_t res = 0, tmppos = this->position;
	bool found = false;

	while(this->canMoveForward(n)){
		bool c1 = memcmp(this->data + this->position, pattern, n);
		if(c1 == 0){
			res = this->position;
			found = true;
			break;
		}else{
			this->seek(1);
		}
	}

	this->position = tmppos;
	if(found == false){
		this->setError();
	}
	return res;
}

/**
 * Test if a bit in the next to be read byte is set.
 * Keep in mind that the bit 0 is the least significant, while bit 8 is the most significant.
 * Example: In 00000010, all bits are set to 0, except bit 1 (that is, the second bit).
 * @param bit The Nth bit to test.
 * @return True if the bit is set, false otherwise.
 */
bool Bits::testBit(unsigned int bit){
	this->unsetError();
	if(!this->canMoveForward() || bit > 7){
		this->setError();
		return false;
	}

	uint8_t byte = (uint8_t) *this->peek(1);
	return byte & (1 << bit);
}

/**
 * Set a bit to 1 in the next to be read byte.
 * Keep in mind that the bit 0 is the least significant, while bit 8 is the most significant.
 * Example: In 00000010, all bits are set to 0, except bit 1 (that is, the second bit).
 * @param bit The Nth bit to set.
 * @return True if the bit was set successfully.
 */
bool Bits::setBit(unsigned int bit){
	this->unsetError();
	if(!this->canMoveForward() || bit > 7){
		this->setError();
		return false;
	}

	uint8_t byte = (uint8_t) *this->peek(1);
	byte |= 1 << bit;
	this->write(&byte, 1);
	this->position--;

	return true;
}

/**
 * Set a bit to 0 in the next to be read byte.
 * Keep in mind that the bit 0 is the least significant, while bit 8 is the most significant.
 * Example: In 00000010, all bits are set to 0, except bit 1 (that is, the second bit).
 * @param bit The Nth bit to set.
 * @return True if the bit was set successfully.
 */
bool Bits::unsetBit(unsigned int bit){
	this->unsetError();
	if(!this->canMoveForward() || bit > 7){
		this->setError();
		return false;
	}

	uint8_t byte = (uint8_t) *this->peek(1);
	byte &= ~(1 << bit);
	this->write(&byte, 1);
	this->position--;

	return true;
}

/**
 * Change the value of a bit in the next to be read byte.
 * Keep in mind that the bit 0 is the least significant, while bit 8 is the most significant.
 * Example: In 00000010, all bits are set to 0, except bit 1 (that is, the second bit).
 * @param bit The Nth bit to modify.
 * @return True if the bit was set successfully.
 */
bool Bits::toggleBit(unsigned int bit){
	this->unsetError();
	if(this->testBit(bit) && !this->checkIfError()){
		return this->unsetBit(bit);
	}else if(!this->checkIfError()){
		return this->setBit(bit);
	}else{
		this->setError();
		return false;
	}
}

/**
 * Print a hexadecimal representation of the SHA1 sum of the data holded by the object.
 */
void Bits::printHash(){
	this->unsetError();
	if(this->hash == NULL){
		this->getHash();
	}

	for(uint64_t i = 0; i < SHA_DIGEST_LENGTH; i++){
		cout << hex << setfill('0') << setw(2) << (int) *(this->hash + i);
	}
}

/**
 * Print N bytes starting from the current position, in a hexadecimal representation.
 * @param n Number of bytes to print.
 */
void Bits::printHex(uint64_t n){
	this->unsetError();
	for(uint64_t i = 0; i < n && this->canMoveForward(); i++){
		cout << hex << setfill('0') << setw(2) << (int) *this->read(1);
	}
}

/**
 * Print N bytes starting from the current position, each byte in it's bits representation.
 * @param n Number of bytes to print.
 */
void Bits::printBits(uint64_t n){
	this->unsetError();
	for(uint64_t i = 0; i < n && this->canMoveForward(); i++){
		cout << bitset<8>(*this->read(1));
	}
}

/**
 * Get a pointer to the data managed by Bits
 * @return A pointer to the data managed by Bits.
 */
unsigned char *Bits::getData(){
	this->unsetError();
	return this->data;
}

/**
 * Getter for the current position.
 * @return The current position.
 */
uint64_t Bits::getPosition(){
	this->unsetError();
	return this->position;
}

/**
 * Get a SHA1 sum of the data holded by this object.
 * @return SHA1 sum.
 */
unsigned char *Bits::getHash(){
	this->unsetError();
	unsigned char *hash = (unsigned char *) malloc(SHA_DIGEST_LENGTH);
	SHA1(this->data, this->max_position, hash);
	this->hash = hash;
	return hash;
}

/**
 * Set the current position.
 * @param pos The position that is desired.
 */
bool Bits::setPosition(uint64_t pos){
	this->unsetError();
	if(this->max_position >= pos){
		this->position = pos;
		return true;
	}else{
		this->setError();
		return false;
	}
}

/**
 * Getter for the maximum position. we can go.
 * @return The maximum position we can go.
 */
uint64_t Bits::getMaxPosition(){
	this->unsetError();
	return this->max_position;
}

/**
 * Set internal error flag to true.
 */
void Bits::setError(){
	this->error = true;
}

/**
 * Set internal error flag to false.
 */
void Bits::unsetError(){
	this->error = false;
}
