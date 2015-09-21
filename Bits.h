#ifndef BITS_H
#define	BITS_H

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <ios>
#include <cmath>
#include <bitset>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cinttypes>
#include <openssl/sha.h>
#include "Utils.h"

#define BYTETOSTRINGPATTERN "%c%c%c%c%c%c%c%c"
#define BYTETOSTRING(byte) \
	byte, \
	*(&byte + 1), \
	*(&byte + 2), \
	*(&byte + 3), \
	*(&byte + 4), \
	*(&byte + 5), \
	*(&byte + 6), \
	*(&byte + 7)

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte) \
	(byte & 0x80 ? 1 : 0), \
	(byte & 0x40 ? 1 : 0), \
	(byte & 0x20 ? 1 : 0), \
	(byte & 0x10 ? 1 : 0), \
	(byte & 0x08 ? 1 : 0), \
	(byte & 0x04 ? 1 : 0), \
	(byte & 0x02 ? 1 : 0), \
	(byte & 0x01 ? 1 : 0)

using namespace std;

typedef unsigned char byte;
typedef struct bit_data {
	byte *data;
	size_t length;
} bit_data;

class Bits {

	public:

		Bits();

		virtual ~Bits();

		bool canMoveBackwards(size_t n = 1);

		bool canMoveForward(size_t n = 1);

		bool checkIfError();

		bool fromFile(char *fname = NULL, ios_base::openmode mode = ios::in | ios::binary);

		bool toFile(char *fname = NULL, size_t offset = 0, size_t size = 0, ios_base::openmode mode = ios::out | ios::binary | ios::trunc);

		bool fromMem(unsigned char *chunk, size_t size);

		void unload();

		unsigned char *read(size_t n, bool reverse = false);

		uint8_t read_uint8();

		uint16_t read_uint16(bool reverse = false);

		uint32_t read_uint32(bool reverse = false);

		uint64_t read_uint64(bool reverse = false);

		Bits *readBits(size_t n_bits, size_t skip_n_bits = 0);

		bool compareBinary(const char *string, size_t check_n_bits, size_t skip_b_bits = 0);

		bool compareHex(const char *string, size_t check_n_bytes, size_t skip_b_bytes = 0);

		unsigned char *peek(size_t n, bool reverse = false);

		bool write(unsigned char *chunk, size_t n, bool patch = true);

		bool seek(size_t n, bool reverse = false);

		size_t findPrevious(unsigned char *pattern, size_t n);

		size_t findNext(unsigned char *pattern, size_t n);

		bool testBit(unsigned int bit);

		bool setBit(unsigned int bit);

		bool unsetBit(unsigned int bit);

		bool toggleBit(unsigned int bit);

		void printHash();

		void printHex(size_t n);

		void printBits(size_t n);

		unsigned char *getData();

		size_t getPosition();

		unsigned char *getHash();

		bool setPosition(size_t pos);

		size_t getMaxPosition();

	private:

		bool is_from_file, error;
		unsigned char *data, *hash;
		size_t position, max_position;

		void unsetError();
		void setError();

};

#endif	/* BITS_H */
