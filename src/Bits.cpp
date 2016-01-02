#include "Bits.h"
using namespace std;

Bits::Bits(char *fname, ios_base::openmode mode) {
	this->init();
	this->fromFile(fname, mode);
}

Bits::Bits(unsigned char *chunk, size_t size) {
	this->init();
	this->fromMem(chunk, size);
}

Bits::Bits(unsigned char *chunk, size_t size, bool auto_free_mem) {
	this->init();
	this->fromMem(chunk, size);
	this->autoFreeMem(auto_free_mem);
}

/*
 * Create a new object of Bits. 'data' will be set to NULL.
 * 'position' and 'max_position' will be set to 0.
 */
void Bits::init() {
	this->auto_free_mem = false;
	this->data = NULL;
	this->position = 0;
	this->max_position = 0;
	this->hash = NULL;
	this->error = false;
	this->is_from_file = false;
}

/*
 * Unload whatever was loaded (if anything was loaded at all).
 * That means that 'data' will be set to NULL  and free-ed if possible*.
 * Also, 'position' and 'max_position' will be set to 0.
 *
 * If possible means that the data will be free-ed if it was malloc-ed in the
 * first place (if the data was loaded from a file). If the data was loaded
 * from a chunk of memory, it won't be free-ed, so take care of it!
 */
Bits::~Bits(){
	if(this->is_from_file && this->data != NULL){
		free(this->data);
	}

	if(!this->is_from_file && this->auto_free_mem && this->data != NULL){
		free(this->data);
	}

	if(this->hash != NULL){
		free(this->hash);
	}
}

/*
 * Make Bits try to auto-free the chunk it holds.
 * This is usually helpfull when you're dealing with
 * Bits objects that were created by another Bits objects.
 * Example:
 *
 * Bits *a = new Bits("Foo bar", 7);
 * Bits *b = a->readBits(2);
 *
 * delete a; // <--- ok
 * delete b; // <--- memory leak, missing free(b->data);
 *
 * --------
 *
 * b->autoFreeMem(true);
 * delete b; // <--- ok
 *
 * Note that this will segfault if set to true on a Bits
 * object that holds a chunk that can't be freed.
 * Example:
 *
 * Bits *a = new Bits("Foo bar", 7);
 * a->autoFreeMem(true);
 * delete a; // <--- segfault
 */
void Bits::autoFreeMem(bool b){
	this->auto_free_mem = b;
}

/*
 * Check if the cursor can go back more than N bytes.
 * @param n_bytes Number of bytes to move backwards
 * @return True if possible, false otherwise.
 */
bool Bits::canMoveBackwards(size_t n_bytes){
	this->unsetError();

	if(n_bytes == 0) return true;
	if(this->position == 0) return false;

	return this->position >= n_bytes;
}

/**
 * Check if the cursor can go forward more than N bytes.
 * @param n_bytes Number of bytes to move forward.
 * @return True if possible, false otherwise.
 */
bool Bits::canMoveForward(size_t n_bytes){
	this->unsetError();

	if(n_bytes == 0) return true;
	if(this->position == this->max_position) return false;

	return (this->position + n_bytes) <= this->max_position;
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
		return state;
	}

	file.open(fname, mode);
	if(!file.is_open()){
		this->setError();
		return state;
	}

	file.seekg(0, ios::end);
	size = file.tellg();

	if(size == -1){
		this->setError();
		return state;
	}

	this->data = (unsigned char *) malloc(size);
	if(this->data == NULL){
		this->setError();
		return state;
	}

	this->position = 0;
	this->max_position = size;
	this->is_from_file = true;

	file.seekg(0, ios::beg);
	file.read((char *) data, size);
	file.close();
	state = true;

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
bool Bits::toFile(char *fname, size_t offset, size_t size, ios_base::openmode mode){
	this->unsetError();
	bool state = false;
	ofstream file;

	if(fname == NULL){
		this->setError();
		return state;
	}

	if(offset > this->max_position){
		this->setError();
		return state;
	}

	if(offset == 0 && size == 0){
		size = this->max_position;
	}else if(offset > 0 && size == 0){
		size = this->max_position - offset;
	}

	if(this->max_position < offset + size){
		this->setError();
		return state;
	}

	file.open(fname, mode);
	if(!file.is_open()){
		this->setError();
		return state;
	}

	file.seekp(0, ios::beg);
	file.write((const char *) this->data + offset, size);
	file.close();
	state = true;

	return state;
}

/**
 * Load a memory chunk of data, the size of N bytes.
 * @param chunk A pointer to the data to be loaded.
 * @param size Number of bytes to load.
 * @return True on success, false otherwise.
 */
bool Bits::fromMem(unsigned char *chunk, size_t size){
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
 * Read N bytes starting from the current position of the data.
 * The current position will be changed to "current position" + n.
 * @param n Number of bytes to read.
 * @param reverse If set to true (default is false) the data will be returned byte-reversed.
 * @return The data that was readen or NULL if something failed.
 */
unsigned char *Bits::read(size_t n, bool reverse){
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
		for(size_t i = n; i > 0; i--){
			tmp[n - i] = this->data[this->position + i - 1];
		}
		this->position += n;
	}else{
		for(size_t i = 0; i < n; i++){
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
		free(c);
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
		free(c);
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
		free(c);
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
		free(c);
	}else{
		this->setError();
	}
	return i;
}

/**
 * Read bits and, optionaly, skip bits from the start. The cursor will be moved by as many bytes as the round up of the (bits to read / 8) and the (bits to skip / 8).
 * @param n_bits Number of bits to read
 * @param skip_n_bits Number of bits to skip from the start. Default 0.
 * @return Bits objec containing the readed bits. Note that if there are no enough bytes to read, the object will be mapped to a 0 length data chunk.
 */
Bits *Bits::readBits(size_t n_bits, size_t skip_n_bits){
	this->unsetError();

	/*
	 * If we got a skip_n_bits bigger than 8, we can skip an entire byte(s)
	 * and then set skip_b_bits to the remining bits.
	 */
	if(skip_n_bits >= 8){
		size_t rem = skip_n_bits / 8;
		this->seek(rem);
		skip_n_bits = skip_n_bits % 8;
	}

	/*
	 * We must allocate enough memory for all the bits, meaning that even if we
	 * must read only 9 bits, we must allocate 2 bytes (16 bits).
	 */
	size_t bytes_to_alloc = (n_bits + 7) / 8;
	size_t bytes_to_read = bytes_to_alloc;
	if(skip_n_bits != 0 && (n_bits + skip_n_bits) > 8) {
		bytes_to_read++;
	}

	if(this->canMoveForward(bytes_to_read) == false) {
		return NULL;
	}

	unsigned char *tmp = this->read(bytes_to_read);
	if(skip_n_bits > 0 && (n_bits + skip_n_bits) <= 8) {
		tmp[0] = tmp[0] << skip_n_bits;
	}else if(skip_n_bits > 0){
		for (size_t i = 0; i < bytes_to_read - 1; i++) {
			tmp[i] = (tmp[i] << skip_n_bits) | (tmp[i+1] >> (8 - skip_n_bits));
		}
	}

	return new Bits(tmp, bytes_to_alloc, true);
}

/**
 * Compare a chunk of data with an binary representation of each bit. The cursor won't be moved.
 * @param string a hexadecimal representation of the tested data. Might contain spaces, but must be uppercase.
 * @param size_t Number of bits to read.
 * @param size_t Number of bits to skip.
 * @return True if data is equal to the given binary represenntation.
 */
bool Bits::compareBinary(const char *string, size_t check_n_bits, size_t skip_n_bits){
	bool match = true;

	/*
	 * Check how many bytes must be read. Note that 9 bits are still 2 bytes.
	 * This doesn't take into account the number of bits that must be skipped.
	 */
	size_t bytes = (check_n_bits + 7) / 8;

	/*
	 * Check how many bytes must be read. This does take into account the number
	 * of bits that must be skipped, so reading 15 bits and skipping 2 bits should
	 * give 3 bytes.
	 */
	size_t bytes_with_skip = (check_n_bits + skip_n_bits + 7) / 8;

	/*
	 * Check if there are enough bytes to read.
	 */
	if(this->canMoveForward(bytes_with_skip) == false) return false;

	unsigned char *bin_string = Utils::removeSpaces(string);
	size_t len = strlen((const char *) bin_string);
	if(!Utils::isValidBinString(bin_string) || len < check_n_bits) {
		free(bin_string);
		return false;
	}

	/*
	 * Save the current position.
	 */
	size_t pos = this->getPosition();

	Bits *data = this->readBits(check_n_bits, skip_n_bits);
	data->autoFreeMem(true);

	char tmp_bin_repr[9];

	for(size_t i = 0; i < bytes; i++) {
		int chars_to_compare = i + 1 == bytes ? check_n_bits % 8 : 8;
		chars_to_compare = chars_to_compare == 0 ? 8 : chars_to_compare;

		uint8_t c = data->read_uint8();

		//Convert c to binary representation with dark magic
		*(unsigned long long*)tmp_bin_repr = 0x3030303030303030ULL +
		(((c * 0x8040201008040201ULL) >> 7) & 0x0101010101010101ULL);

		if(memcmp(tmp_bin_repr, &bin_string[i * 8], chars_to_compare) != 0) {
			match = false;
			break;
		}
	}

	delete data;
	free(bin_string);

	/*
	 * Restore the position in which the cursor was before the compare process.
	 */
	this->setPosition(pos);

	return match;
}

/**
 * Compare a chunk of data with an hexadecimal representation of each byte. The cursor won't be moved.
 * @param string a hexadecimal representation of the tested data. Might contain spaces, but must be uppercase.
 * @param size_t Number of bytes to read.
 * @param size_t Number of bytes to skip.
 * @return True if data is equal to the given hex represenntation.
 */
bool Bits::compareHex(const char *string, size_t check_n_bytes, size_t skip_n_bytes){
	bool match = true;
	/*
	 * Check if there are enough bytes to read.
	 */
	if(this->canMoveForward(check_n_bytes) == false) return false;

	unsigned char *hex_string = Utils::removeSpaces(string);
	size_t len = strlen((const char *) hex_string);

	/*
	 * If the received string is not a valid hex representation, quit.
	 */
	if(!Utils::isValidHexString(hex_string) || len < check_n_bytes * 2) {
		free(hex_string);
		return false;
	}

	/*
	 * Save the current position.
	 */
	size_t pos = this->getPosition();

	this->seek(skip_n_bytes);
	unsigned char *data = this->read(check_n_bytes);

	char tmp_hex_repr[3];

	for(size_t i = 0; i < check_n_bytes; i++) {
		sprintf((char *) &tmp_hex_repr, "%X", data[i]);

		if(memcmp(tmp_hex_repr, &hex_string[i * 2], 2) != 0) {
			match = false;
			break;
		}
	}

	free(data);
	free(hex_string);

	/*
	 * Restore the position in which the cursor was before the compare process.
	 */
	this->setPosition(pos);

	return match;
}

/**
 * Read N bytes starting from the current position of the data, without changing the position.
 * @param n Number of bytes to read.
 * @param reverse If set to true (default is false) the data will be returned byte-reversed.
 * @return The data that was readen or NULL if something failed.
 */
unsigned char *Bits::peek(size_t n, bool reverse){
	this->unsetError();

	if(n == 0) return NULL;

	if(!this->canMoveForward(n)){
		this->setError();
		return NULL;
	}

	size_t tmppos = this->position;
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
bool Bits::write(unsigned char *chunk, size_t n, bool patch){
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
bool Bits::seek(size_t n, bool reverse){
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
size_t Bits::findPrevious(unsigned char *pattern, size_t n){
	this->unsetError();
	size_t res = 0, tmppos = this->position;
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
size_t Bits::findNext(unsigned char *pattern, size_t n){
	this->unsetError();
	size_t res = 0, tmppos = this->position;
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

	uint8_t byte = this->read_uint8();
	this->seek(1, true);
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

	uint8_t byte = this->read_uint8();
	this->seek(1, true);
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

	uint8_t byte = this->read_uint8();
	this->seek(1, true);
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

	for(size_t i = 0; i < SHA_DIGEST_LENGTH; i++){
		cout << hex << setfill('0') << setw(2) << (int) *(this->hash + i);
	}
}

/**
 * Print N bytes starting from the current position, in a hexadecimal representation. Nothing will be printed if the requested number of bytes is bigger than the remaining data. Cursor won't be moved.
 * @param n Number of bytes to print.
 */
void Bits::printAsHex(size_t n){
	this->unsetError();
	if(this->canMoveForward(n) == false) return;
	unsigned char *c;
	for(size_t i = 0; i < n; i++){
		c = this->read(1);
		cout << hex << setfill('0') << setw(2) << (int) *c;
		if(i + 1 < n) {
			cout << ' ';
		}
		free(c);
	}
	this->seek(n, true);

}

/**
 * Print N bytes starting from the current position, each byte in it's binary representation. Nothing will be printed if the requested number of bytes is bigger than the remaining data. Cursor won't be moved.
 * @param n Number of bytes to print.
 */
void Bits::printAsBinary(size_t n){
	this->unsetError();
	if(this->canMoveForward(n) == false) return;
	unsigned char *c;
	for(size_t i = 0; i < n; i++){
		c = this->read(1);
		cout << bitset<8>(*c);
		if(i + 1 < n) {
			cout << ' ';
		}
		free(c);
	}
	this->seek(n, true);
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
size_t Bits::getPosition(){
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
bool Bits::setPosition(size_t pos){
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
size_t Bits::getMaxPosition(){
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
