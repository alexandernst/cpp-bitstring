#ifndef BITS_H
#define	BITS_H

#include <bitset>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>

class Bits {

	public:

		Bits();

		virtual ~Bits();

		bool fromFile(char *fname, const char *mode = "rb");

		bool toFile(char *fname, const char *mode = "rb");

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

