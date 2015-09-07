#ifndef BITS_H
#define	BITS_H

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <ios>
#include <bitset>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <inttypes.h>
#include <openssl/sha.h>

using namespace std;

class Bits {

	public:

		Bits();

		virtual ~Bits();

		bool canMoveBackwards(int64_t n = 1);

		bool canMoveForward(int64_t n = 1);

		bool checkIfError();

		bool fromFile(char *fname = NULL, ios_base::openmode mode = ios::in | ios::binary);

		bool toFile(char *fname = NULL, int64_t offset = 0, int64_t size = -1, ios_base::openmode mode = ios::out | ios::binary | ios::trunc);

		bool fromMem(unsigned char *chunk, int64_t size);

		void unload();

		unsigned char *read(int64_t n, bool reverse = false);

		uint8_t read_uint8();

		uint16_t read_uint16(bool reverse = false);

		uint32_t read_uint32(bool reverse = false);

		uint64_t read_uint64(bool reverse = false);

		unsigned char *peek(int64_t n, bool reverse = false);

		bool write(unsigned char *chunk, int64_t n, bool patch = true);

		bool seek(int64_t n);

		int64_t findPrevious(unsigned char *pattern, int64_t n);

		int64_t findNext(unsigned char *pattern, int64_t n);

		bool testBit(unsigned int bit);

		bool setBit(unsigned int bit);

		bool unsetBit(unsigned int bit);

		bool toggleBit(unsigned int bit);

		void printHash();

		void printHex(int64_t n);

		void printBits(int64_t n);

		unsigned char *getData();

		int64_t getPosition();

		unsigned char *getHash();

		bool setPosition(int64_t pos);

		int64_t getMaxPosition();

	private:

		bool is_from_file, error;
		unsigned char *data, *hash;
		int64_t position, max_position;

		void unsetError();
		void setError();

};

#endif	/* BITS_H */

