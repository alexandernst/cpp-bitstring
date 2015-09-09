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

using namespace std;

class Bits {

	public:

		Bits();

		virtual ~Bits();

		bool canMoveBackwards(uint64_t n = 1);

		bool canMoveForward(uint64_t n = 1);

		bool checkIfError();

		bool fromFile(char *fname = NULL, ios_base::openmode mode = ios::in | ios::binary);

		bool toFile(char *fname = NULL, uint64_t offset = 0, uint64_t size = 0, ios_base::openmode mode = ios::out | ios::binary | ios::trunc);

		bool fromMem(unsigned char *chunk, uint64_t size);

		void unload();

		unsigned char *read(uint64_t n, bool reverse = false);

		uint8_t read_uint8();

		uint16_t read_uint16(bool reverse = false);

		uint32_t read_uint32(bool reverse = false);

		uint64_t read_uint64(bool reverse = false);

		unsigned char *peek(uint64_t n, bool reverse = false);

		bool write(unsigned char *chunk, uint64_t n, bool patch = true);

		bool seek(uint64_t n, bool reverse = false);

		uint64_t findPrevious(unsigned char *pattern, uint64_t n);

		uint64_t findNext(unsigned char *pattern, uint64_t n);

		bool testBit(unsigned int bit);

		bool setBit(unsigned int bit);

		bool unsetBit(unsigned int bit);

		bool toggleBit(unsigned int bit);

		void printHash();

		void printHex(uint64_t n);

		void printBits(uint64_t n);

		unsigned char *getData();

		uint64_t getPosition();

		unsigned char *getHash();

		bool setPosition(uint64_t pos);

		uint64_t getMaxPosition();

	private:

		bool is_from_file, error;
		unsigned char *data, *hash;
		uint64_t position, max_position;

		void unsetError();
		void setError();

};

#endif	/* BITS_H */
