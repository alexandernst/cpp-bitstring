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
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cinttypes>
#include <openssl/sha.h>
#include "Utils.h"

using namespace std;

class Bits {

	public:

		Bits(char *fname = NULL, ios_base::openmode mode = ios::in | ios::binary);

		Bits(unsigned char *chunk, size_t size);

		Bits(unsigned char *chunk, size_t size, bool auto_free_mem);

		virtual ~Bits();

		void autoFreeMem(bool auto_free_mem);

		bool canMoveBackwards(size_t n_bytes = 1);

		bool canMoveForward(size_t n_bytes = 1);

		bool checkIfError();

		bool toFile(char *fname = NULL, size_t offset = 0, size_t size = 0, ios_base::openmode mode = ios::out | ios::binary | ios::trunc);

		void clear();

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

		unsigned char *getAsHex(size_t num_bytes);

		void printAsHex(size_t num_bytes);

		unsigned char *getAsBinary(size_t num_bytes);

		void printAsBinary(size_t n);

		unsigned char *getData();

		size_t getPosition();

		unsigned char *getHash();

		bool setPosition(size_t pos);

		size_t getMaxPosition();

	private:

		bool auto_free_mem, is_from_file, error;
		unsigned char *data, *hash;
		size_t position, max_position;

		void init();
		bool fromFile(char *fname = NULL, ios_base::openmode mode = ios::in | ios::binary);
		bool fromMem(unsigned char *chunk, size_t size);

		void unsetError();
		void setError();

};

#endif	/* BITS_H */
