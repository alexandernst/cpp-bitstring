#ifndef BITS_H
#define	BITS_H

#include <bitset>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <inttypes.h>

using namespace std;

/**
 * Create a new object of Bits. 'data' will be set to NULL.
 * 'position' and 'max_position' will be set to 0.
 */
class Bits {

	public:

		Bits();

		virtual ~Bits();

		bool fromFile(char *fname = NULL, ios_base::openmode mode = ios::in | ios::binary);

		bool toFile(char *fname = NULL, int64_t offset = 0, int64_t size = -1, ios_base::openmode mode = ios::out | ios::binary | ios::trunc);

		bool fromMem(unsigned char *chunk, int64_t size);

		void unload();

		unsigned char *read(int64_t n, bool reverse = false);

		unsigned char *peek(int64_t n, bool reverse = false);

		bool write(unsigned char *chunk, int64_t n, bool patch = true);

		bool seek(int64_t n);

		int64_t findPrevious(unsigned char *pattern, int64_t n);

		int64_t findNext(unsigned char *pattern, int64_t n);

		bool testBit(unsigned int bit);

		bool setBit(unsigned int bit);

		bool unsetBit(unsigned int bit);

		bool toggleBit(unsigned int bit);

		void printHex(int64_t n);

		void printBits(int64_t n);

		unsigned char *getData();

		int64_t getPosition();

		bool setPosition(int64_t pos);

		int64_t getMaxPosition();

	private:

		unsigned char *data;
		int64_t position, max_position;

		bool canMoveBackwards(int64_t n = 1);

		bool canMoveForward(int64_t n = 1);

};

#endif	/* BITS_H */

