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
}

/**
 * Destroying the Bits object will call 'unload()'.
 */
Bits::~Bits(){
	this->unload();
}

/**
 * Read data from file.
 * @param fname The path/name of the file.
 * @param mode Optional, open mode. Default is "rb".
 * @return True if data was successfully readen, otherwise false.
 */
bool Bits::fromFile(char *fname, ios_base::openmode mode){
	bool state = false;
	streampos size;
	ifstream file(fname, mode);

	if(file.is_open()){
		size = file.tellg();

		this->data = (unsigned char *) malloc(size);
		if(this->data == NULL){
			state = false;
		}

		this->position = 0;
		this->max_position = size;

		file.seekg(0, ios::beg);
		file.read((char *)data, size);
		file.close();

		state = true;
	}

	return state;
}

/**
 * Write data to file.
 * @param fname The path/name of the file.
 * @param mode Optional, open mode. Default is "wb".
 * @return True if data was successfully written, otherwise false.
 */
bool Bits::toFile(char *fname, const char *mode){

}

/**
 * Load a memory chunk of data, the size of N bytes.
 * @param chunk A  pointer to the data to be loaded.
 * @param size Number of bytes to load.
 * @return True on success, false otherwise.
 */
bool Bits::fromMem(unsigned char *chunk, int64_t size){
	if(chunk == NULL){
		return false;
	}else{
		this->data = chunk;
		this->position = 0;
		this->max_position = size;
		return true;
	}
}

/**
 * Unload whatever was loaded (if anything was loaded at all).
 * That means that 'data' will be set to NULL.
 * Also, 'position' and 'max_position' will be set to 0.
 */
void Bits::unload(){
	this->data = NULL;
	this->position = 0;
	this->max_position = 0;
}

/**
 * Read N bytes starting from the current position of the data.
 * The current position will be changed to <current position> + n.
 * @param n Number of bytes to read.
 * @param reverse If set to true (default is false) the data will be returned byte-reversed.
 * @return The data that was readen or NULL if something failed.
 */
unsigned char *Bits::read(int64_t n, bool reverse){
	if(!this->canMoveForward(n)){
		return NULL;
	}

	unsigned char *tmp = (unsigned char *) malloc(n);

	if(!tmp){
		return NULL;
	}

	if(reverse){
		for(int64_t i = n - 1; i >= 0; i--){
			tmp[n - (i + 1)] = this->data[this->position + i];
		}
		this->position += n;
	}else{
		for(int64_t i = 0; i < n; i++){
			tmp[i] = this->data[this->position];
			this->position++;
		}
	}

	return tmp;
}

/**
 * Read N bytes starting from the current position of the data, without changing the position.
 * @param n Number of bytes to read.
 * @param reverse If set to true (default is false) the data will be returned byte-reversed.
 * @return The data that was readen or NULL if something failed.
 */
unsigned char *Bits::peek(int64_t n, bool reverse){
	if(!this->canMoveForward(n)){
		return NULL;
	}

	int64_t tmppos = this->position;
	unsigned char *tmp = this->read(n, reverse);
	this->position = tmppos;

	return tmp;
}

/**
 * Write N bytes of data read from chunk, starting from the current position of the data.
 * The current position will be changed to <current position> + n.
 * @param chunk The data we want to write.
 * @param n Number of bytes we want to write from the chunk.
 * @param patch If set to false (default is true) data will be inserted without replacing, instead of patching.
 * @return True on success, false otherwise.
 */
bool Bits::write(unsigned char *chunk, int64_t n, bool patch){
	if(patch && this->canMoveForward(n)){

		memcpy(this->data + this->position, chunk, n);

	}else if(!patch){

		unsigned char *newdata = (unsigned char*)malloc(this->max_position + n);

		//Copy from position 0 to current position
		memcpy(newdata, this->data, this->position);

		//Insert new data
		memcpy(newdata + this->position, chunk, n);

		//Finish copying until the end
		memcpy(newdata + this->position + n, this->data + this->position, this->max_position - this->position);

		try{
			//free(this->data);
		}catch(...){
			//Oops...
		}

		this->data = newdata;
		this->max_position += n;

	}else{

		return false;

	}

	this->position += n;
	return true;
}

/**
 * Seek N bytes in the currently loaded data.
 * If N is negative, the position will be decreased.
 * If the operation exceeds the minimum or maximum position, false will be returned.
 * @param n Number of bytes to seek.
 * @return True is the operation was successful, false otherwise.
 */
bool Bits::seek(int64_t n){
	if(
			(n >= 0 && this->canMoveForward(n)) ||
			(n <= 0 && this->canMoveBackwards(n))
	){
		this->position += n;
		return true;
	}else{
		return false;
	}
}

/**
 * Find the nearest match to a given pattern in the data behind the current position.
 * The current position will remain unchanged.
 * @param pattern The byte(s) pattern we are looking for.
 * @param n The number of bytes the pattern is long.
 * @return Position/offset or -1 if nothing was found.
 */
int64_t Bits::findPrevious(unsigned char *pattern, int64_t n){
	int64_t res = -1, tmppos = this->position;

	while(this->canMoveBackwards()){;
		if(!this->canMoveForward(n)){
			this->seek(-1);
			continue;
		}

		bool c1 = memcmp(this->data + this->position, pattern, n);
		if(c1 == 0){
			res = this->position;
			break;
		}else{
			this->seek(-1);
		}
	}

	this->position = tmppos;
	return res;
}

/**
 * Find the nearest match to a given pattern in the data starting at the current position.
 * This, effectively, means that if you search something which is already right next
 * to the current position you'll keep "finding" it forever and ever. Just use "seek" or
 * "setPosition" to go to <current position> + 1 and continue searching from there.
 * The current position will remain unchanged.
 * @param pattern The byte(s) pattern we are looking for.
 * @param n The number of bytes the pattern is long.
 * @return Position/offset or -1 if nothing was found.
 */
int64_t Bits::findNext(unsigned char *pattern, int64_t n){
	int64_t res = -1, tmppos = this->position;

	while(this->canMoveForward(n)){
		bool c1 = memcmp(this->data + this->position, pattern, n);
		if(c1 == 0){
			res = this->position;
			break;
		}else{
			this->seek(1);
		}
	}

	this->position = tmppos;
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
	if(!this->canMoveForward() || bit > 7){
		return false;
	}

	uint8_t byte = (uint8_t) *this->peek(1);
	return byte & (1 << bit);
}

/**
 * Test a bit to 1 in the next to be read byte.
 * Keep in mind that the bit 0 is the least significant, while bit 8 is the most significant.
 * Example: In 00000010, all bits are set to 0, except bit 1 (that is, the second bit).
 * @param bit The Nth bit to set.
 * @return True if the bit is set, false otherwise.
 */
bool Bits::setBit(unsigned int bit){
	if(!this->canMoveForward() || bit > 7){
		return false;
	}

	uint8_t byte = (uint8_t) *this->peek(1);
	byte |= 1 << bit;
	this->write(&byte, 1);
	this->position--;

	return true;
}

bool Bits::unsetBit(unsigned int bit){
	if(!this->canMoveForward() || bit > 7){
		return false;
	}

	uint8_t byte = (uint8_t) *this->peek(1);
	byte &= ~(1 << bit);
	this->write(&byte, 1);
	this->position--;

	return true;
}

bool Bits::toggleBit(unsigned int bit){
	if(this->testBit(bit)){
		return this->unsetBit(bit);
	}else{
		return this->setBit(bit);
	}
}

/**
 * Print N bytes starting from the current position, in a hexadecimal representation.
 * @param n Number of bytes to print.
 */
void Bits::printHex(int64_t n){
	for(int64_t i = 0; i < n && this->canMoveForward(); i++){
		cout << hex << (int) *this->read(1);
	}
}

/**
 * Print N bytes starting from the current position, each byte in it's bits representation.
 * @param n Number of bytes to print.
 */
void Bits::printBits(int64_t n){
	for(int64_t i = 0; i < n && this->canMoveForward(); i++){
		cout << bitset<8>(*this->read(1));
	}
}

/**
 * Get a pointer to the data managed by Bits
 * @return A pointer to the data managed by Bits.
 */
unsigned char *Bits::getData(){
	return this->data;
}

/**
 * Getter for the current position.
 * @return The current position.
 */
int64_t Bits::getPosition(){
	return this->position;
}

/**
 * Set the current position.
 * @param The position that is desired.
 */
bool Bits::setPosition(int64_t pos){
	if(this->max_position >= pos){
		this->position = pos;
		return true;
	}else{
		return false;
	}
}

/**
 * Getter for the maximum position. we can go.
 * @return The maximum position we can go.
 */
int64_t Bits::getMaxPosition(){
	return this->max_position;
}

/******************************************************************************/

bool Bits::canMoveBackwards(int64_t n){
	if(this->position == 0 || this->position < n){
		return false;
	}

	return (this->position + n) >= 0;
}

bool Bits::canMoveForward(int64_t n){
	return (this->position + n) <= this->max_position;
}