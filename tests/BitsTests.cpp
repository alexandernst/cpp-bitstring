#include "BitsTests.h"
#include "Bits.h"
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(BitsTests);

BitsTests::BitsTests(){
}

BitsTests::~BitsTests(){
}

void BitsTests::setUp(){
	//Starting tests...
}

void BitsTests::tearDown(){
	//Finishing tests...
}

void BitsTests::testFromFile(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	char tmpfn[L_tmpnam];
	CPPUNIT_ASSERT(tmpnam(tmpfn) != NULL);

	ofstream tmpf(tmpfn);
	CPPUNIT_ASSERT(tmpf.is_open() == true);
	tmpf << chunk;
	tmpf.close();

	Bits bits;
	bool result = bits.fromFile(tmpfn);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(bits.getMaxPosition() == size);

	unsigned char *f2 = bits.read(2);
	unsigned int c_f2 = memcmp(f2, "Th", 2);
	CPPUNIT_ASSERT(c_f2 == 0);

	CPPUNIT_ASSERT(remove(tmpfn) == 0);
}

void BitsTests::testToFile(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	char tmpfn[L_tmpnam];
	CPPUNIT_ASSERT(tmpnam(tmpfn) != NULL);

	Bits bits;
	bool readFromFile = bits.fromMem(chunk, size);
	CPPUNIT_ASSERT(readFromFile);
	CPPUNIT_ASSERT(bits.getMaxPosition() == size);

	bool writeToFile = bits.toFile(tmpfn);
	CPPUNIT_ASSERT(writeToFile);

	Bits bits2;
	readFromFile = bits2.fromFile(tmpfn);
	CPPUNIT_ASSERT(readFromFile);
	CPPUNIT_ASSERT(bits2.getMaxPosition() == size);
	CPPUNIT_ASSERT(memcmp(chunk, bits2.getData(), size) == 0);

	writeToFile = bits.toFile(tmpfn, 14, 1);
	CPPUNIT_ASSERT(writeToFile);

	readFromFile = bits2.fromFile(tmpfn);
	CPPUNIT_ASSERT(readFromFile);
	CPPUNIT_ASSERT(bits2.getMaxPosition() == 1);
	CPPUNIT_ASSERT(memcmp("!", bits2.getData(), 1) == 0);

	writeToFile = bits.toFile(tmpfn, 5, 2);
	CPPUNIT_ASSERT(writeToFile);

	readFromFile = bits2.fromFile(tmpfn);
	CPPUNIT_ASSERT(readFromFile);
	CPPUNIT_ASSERT(bits2.getMaxPosition() == 2);
	CPPUNIT_ASSERT(memcmp("is", bits2.getData(), 2) == 0);

	writeToFile = bits.toFile(tmpfn, 15);
	CPPUNIT_ASSERT(writeToFile);

	readFromFile = bits2.fromFile(tmpfn);
	CPPUNIT_ASSERT(readFromFile);
	CPPUNIT_ASSERT(bits2.getMaxPosition() == 0);

	writeToFile = bits.toFile(tmpfn, 14, 2);
	CPPUNIT_ASSERT(!writeToFile);

	writeToFile = bits.toFile(tmpfn, 0, 0);
	CPPUNIT_ASSERT(writeToFile);

	readFromFile = bits2.fromFile(tmpfn);
	CPPUNIT_ASSERT(readFromFile);
	CPPUNIT_ASSERT(bits2.getMaxPosition() == 0);

	CPPUNIT_ASSERT(remove(tmpfn) == 0);
}

void BitsTests::testFromMem(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bool result = bits.fromMem(chunk, size);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(bits.getMaxPosition() == size);

	unsigned char *f2 = bits.read(2);
	unsigned int c_f2 = memcmp(f2, "Th", 2);
	CPPUNIT_ASSERT(c_f2 == 0);

	unsigned char *n4 = bits.read(4);
	unsigned int c_n4 = memcmp(n4, "is i", 4);
	CPPUNIT_ASSERT(c_n4 == 0);

	unsigned char *n1 = bits.read(1);
	unsigned int c_n1 = memcmp(n1, "s", 1);
	CPPUNIT_ASSERT(c_n1 == 0);

	CPPUNIT_ASSERT(bits.getPosition() == 7);
}

void BitsTests::testUnload(){
	int64_t size = 10;
	unsigned char chunk[size];

	Bits bits;
	bits.fromMem(chunk, size);
	bits.unload();

	CPPUNIT_ASSERT(bits.getData() == NULL);
	CPPUNIT_ASSERT(bits.getMaxPosition() == 0);
	CPPUNIT_ASSERT(bits.getPosition() == 0);
}

void BitsTests::testRead(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	bool s1 = bits.seek(10);
	CPPUNIT_ASSERT(s1);

	unsigned char *n5 = bits.read(5, true);
	unsigned int c_n5 = memcmp(n5, "!tset", 5);
	CPPUNIT_ASSERT(c_n5 == 0);

	bool s2 = bits.seek(-10);
	CPPUNIT_ASSERT(s2);

	unsigned char *n2 = bits.read(2, true);
	unsigned int c_n2 = memcmp(n2, "si", 2);
	CPPUNIT_ASSERT(c_n2 == 0);

	CPPUNIT_ASSERT(bits.getPosition() == 7);
}

void BitsTests::testPeek(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	bool s1 = bits.seek(16);
	CPPUNIT_ASSERT(s1 == false);

	unsigned char *n5 = bits.peek(5, true);
	unsigned int c_n5 = memcmp(n5, " sihT", 5);
	CPPUNIT_ASSERT(c_n5 == 0);

	unsigned char *n1 = bits.peek(1);
	unsigned int c_n1 = memcmp(n1, bits.peek(1), 1);
	CPPUNIT_ASSERT(c_n1 == 0);

	CPPUNIT_ASSERT(bits.getPosition() == 0);
}

void BitsTests::testWrite(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	bits.write((unsigned char *)"That", 4);

	unsigned int c1 = memcmp(bits.getData(), "That is a test!", size);
	CPPUNIT_ASSERT(c1 == 0);

	bits.write((unsigned char *)" string", 7, false);

	unsigned int c2 = memcmp(bits.getData(), "That string is a test!", size + 7);
	CPPUNIT_ASSERT(c2 == 0);
}

void BitsTests::testSeek(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	bool s1 = bits.seek(6);
	CPPUNIT_ASSERT(s1);

	unsigned char *n3 = bits.read(3);
	unsigned int c_n3 = memcmp(n3, "s a", 3);
	CPPUNIT_ASSERT(c_n3 == 0);

	bool s2 = bits.seek(6);
	CPPUNIT_ASSERT(s2);

	unsigned char *n1 = bits.read(1);
	CPPUNIT_ASSERT(n1 == NULL);

	bool s3 = bits.seek(-2);
	CPPUNIT_ASSERT(s3);

	unsigned char *p2 = bits.read(2);
	unsigned int c_p2 = memcmp(p2, "t!", 2);
	CPPUNIT_ASSERT(c_p2 == 0);

	bool s4 = bits.seek(-15);
	CPPUNIT_ASSERT(s4);

	bool s5 = bits.seek(1);
	CPPUNIT_ASSERT(s5);

	bool s6 = bits.seek(-2);
	CPPUNIT_ASSERT(s6 == false);
}

void BitsTests::testFindPrevious(){
	unsigned char chunk[] = "This is a really long test with some 28452 random data in 9352 it!!?";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	int64_t p1 = bits.findNext((unsigned char *) "!", 1);
	CPPUNIT_ASSERT(p1 == 65);

	bits.setPosition(p1 + 1);

	int64_t p2 = bits.findNext((unsigned char *) "!", 1);
	CPPUNIT_ASSERT(p2 == 66);

	bits.setPosition(p2);

	int64_t p3 = bits.findPrevious((unsigned char *) "45", 2);
	CPPUNIT_ASSERT(p3 == 39);

	bits.setPosition(bits.getMaxPosition());

	int64_t p4 = bits.findPrevious((unsigned char *) "!?", 2);
	CPPUNIT_ASSERT(p4 == 66);

	bits.setPosition(0);

	int64_t p5 = bits.findPrevious((unsigned char *) "T", 1);
	CPPUNIT_ASSERT(p5 == -1);
}

void BitsTests::testFindNext(){
	unsigned char chunk[] = "This is a really long test with some 28452 random data in 9352 it!!?";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	int64_t p1 = bits.findNext((unsigned char *) "a", 1);
	CPPUNIT_ASSERT(p1 == 8);

	int64_t p2 = bits.findNext((unsigned char *) "rand", 4);
	CPPUNIT_ASSERT(p2 == 43);

	bits.setPosition(p2);

	int64_t p3 = bits.findNext((unsigned char *) "52", 2);
	CPPUNIT_ASSERT(p3 == 60);

	bits.setPosition(bits.getMaxPosition());

	int64_t p4 = bits.findNext((unsigned char *) "?", 1);
	CPPUNIT_ASSERT(p4 == -1);
}

void BitsTests::testBit(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	bool b1 = bits.testBit(0);
	CPPUNIT_ASSERT(b1 == false);

	bool b2 = bits.testBit(1);
	CPPUNIT_ASSERT(b2 == false);

	bool b3 = bits.testBit(2);
	CPPUNIT_ASSERT(b3);

	bool b4 = bits.testBit(3);
	CPPUNIT_ASSERT(b4 == false);

	bool b5 = bits.testBit(4);
	CPPUNIT_ASSERT(b5);

	bool b6 = bits.testBit(5);
	CPPUNIT_ASSERT(b6 == false);

	bool b7 = bits.testBit(6);
	CPPUNIT_ASSERT(b7);

	bool b8 = bits.testBit(7);
	CPPUNIT_ASSERT(b8 == false);
}

void BitsTests::testSetBit(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	bits.setBit(0);
	bits.setBit(1);

	unsigned int c1 = memcmp(bits.read(1), "W", 1);
	CPPUNIT_ASSERT(c1 == 0);

	bits.seek(1);

	bits.setBit(0);
	bits.setBit(1);

	unsigned int c2 = memcmp(bits.read(1), "k", 1);
	CPPUNIT_ASSERT(c2 == 0);
}

void BitsTests::testUnsetBit(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	bits.unsetBit(4);

	unsigned int c1 = memcmp(bits.read(1), "D", 1);
	CPPUNIT_ASSERT(c1 == 0);

	bits.seek(1);

	bits.unsetBit(5);

	unsigned int c2 = memcmp(bits.read(1), "I", 1);
	CPPUNIT_ASSERT(c2 == 0);
}

void BitsTests::testToggleBit(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	bits.toggleBit(0);
	bits.toggleBit(1);

	unsigned int c1 = memcmp(bits.read(1), "W", 1);
	CPPUNIT_ASSERT(c1 == 0);

	bits.seek(7);

	bits.unsetBit(0);

	unsigned int c2 = memcmp(bits.read(1), "`", 1);
	CPPUNIT_ASSERT(c2 == 0);
}

void BitsTests::testPrintHash(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	//Capture cout
	ostringstream out;
	streambuf *orig_buf(cout.rdbuf(out.rdbuf()));

	bits.printHash();

	//Restore cout
	cout.rdbuf(orig_buf);

	unsigned int c1 = memcmp(out.str().c_str(), "8b6ccb43dca2040c3cfbcd7bfff0b387d4538c33", SHA_DIGEST_LENGTH);
	CPPUNIT_ASSERT(c1 == 0);
}

void BitsTests::testPrintHex(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	//Capture cout
	ostringstream out;
	streambuf *orig_buf(cout.rdbuf(out.rdbuf()));

	bits.printHex(size);

	//Restore cout
	cout.rdbuf(orig_buf);

	unsigned int c1 = memcmp(out.str().c_str(), "546869732069732061207465737421", size * 2);
	CPPUNIT_ASSERT(c1 == 0);
}

void BitsTests::testPrintBits(){
	unsigned char chunk[] = "This is a test!";
	int64_t size = sizeof(chunk) - 1;

	Bits bits;
	bits.fromMem(chunk, size);

	//Capture cout
	ostringstream out;
	streambuf *orig_buf(cout.rdbuf(out.rdbuf()));

	bits.printBits(2);

	//Restore cout
	cout.rdbuf(orig_buf);

	unsigned int c1 = memcmp(out.str().c_str(), "0101010001101000", 16);
	CPPUNIT_ASSERT(c1 == 0);
}