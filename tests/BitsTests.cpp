#include "BitsTests.h"
#include "src/Bits.h"
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
	size_t size = sizeof(chunk) - 1;

	const char *tplt = "/tmp/cppbitstring.XXXXXX";
	char *tmpfn = (char *) calloc(1, strlen(tplt) + 1);
	memcpy(tmpfn, tplt, strlen(tplt));

	ofstream tmpf(tmpfn);
	CPPUNIT_ASSERT(tmpf.is_open() == true);
	tmpf << chunk;
	tmpf.close();

	Bits *bits = new Bits(tmpfn);
	bool result = bits->checkIfError();
	CPPUNIT_ASSERT(result == false);
	CPPUNIT_ASSERT(bits->getMaxPosition() == size);

	unsigned char *f2 = bits->read(2);
	unsigned int c_f2 = memcmp(f2, "Th", 2);
	CPPUNIT_ASSERT(c_f2 == 0);
	free(f2);

	CPPUNIT_ASSERT(remove(tmpfn) == 0);
	free(tmpfn);
	delete bits;
}

void BitsTests::testToFile(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	const char *tplt = "/tmp/cppbitstring.XXXXXX";
	char *tmpfn = (char *) calloc(1, strlen(tplt) + 1);
	memcpy(tmpfn, tplt, strlen(tplt));

	Bits *bits = new Bits(chunk, size);
	bool readFromFile = bits->checkIfError();
	CPPUNIT_ASSERT(readFromFile == false);
	CPPUNIT_ASSERT(bits->getMaxPosition() == size);

	bool writeToFile = bits->toFile(tmpfn);
	CPPUNIT_ASSERT(writeToFile);

	Bits *bits2 = new Bits(tmpfn);
	readFromFile = bits2->checkIfError();
	CPPUNIT_ASSERT(readFromFile == false);
	CPPUNIT_ASSERT(bits2->getMaxPosition() == size);
	CPPUNIT_ASSERT(memcmp(chunk, bits2->getData(), size) == 0);

	writeToFile = bits->toFile(tmpfn, 14, 1);
	CPPUNIT_ASSERT(writeToFile);

	delete bits2;
	bits2 = new Bits(tmpfn);
	readFromFile = bits2->checkIfError();
	CPPUNIT_ASSERT(readFromFile == false);
	CPPUNIT_ASSERT(bits2->getMaxPosition() == 1);
	CPPUNIT_ASSERT(memcmp("!", bits2->getData(), 1) == 0);

	writeToFile = bits->toFile(tmpfn, 5, 2);
	CPPUNIT_ASSERT(writeToFile);

	delete bits2;
	bits2 = new Bits(tmpfn);
	readFromFile = bits2->checkIfError();
	CPPUNIT_ASSERT(readFromFile == false);
	CPPUNIT_ASSERT(bits2->getMaxPosition() == 2);
	CPPUNIT_ASSERT(memcmp("is", bits2->getData(), 2) == 0);

	writeToFile = bits->toFile(tmpfn, 15);
	CPPUNIT_ASSERT(writeToFile);

	delete bits2;
	bits2 = new Bits(tmpfn);
	readFromFile = bits2->checkIfError();
	CPPUNIT_ASSERT(readFromFile == false);
	CPPUNIT_ASSERT(bits2->getMaxPosition() == 0);

	writeToFile = bits->toFile(tmpfn, 14, 2);
	CPPUNIT_ASSERT(!writeToFile);

	CPPUNIT_ASSERT(remove(tmpfn) == 0);

	free(tmpfn);
	delete bits;
	delete bits2;
}

void BitsTests::testFromMem(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);
	bool result = bits->checkIfError();
	CPPUNIT_ASSERT(result == false);
	CPPUNIT_ASSERT(bits->getMaxPosition() == size);

	unsigned char *f2 = bits->read(2);
	unsigned int c_f2 = memcmp(f2, "Th", 2);
	CPPUNIT_ASSERT(c_f2 == 0);
	free(f2);

	unsigned char *n4 = bits->read(4);
	unsigned int c_n4 = memcmp(n4, "is i", 4);
	CPPUNIT_ASSERT(c_n4 == 0);
	free(n4);

	unsigned char *n1 = bits->read(1);
	unsigned int c_n1 = memcmp(n1, "s", 1);
	CPPUNIT_ASSERT(c_n1 == 0);
	free(n1);

	CPPUNIT_ASSERT(bits->getPosition() == 7);

	delete bits;
}

void BitsTests::testRead(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	bool s1 = bits->seek(10);
	CPPUNIT_ASSERT(s1);
	unsigned char *n5 = bits->read(5, true);
	unsigned int c_n5 = memcmp(n5, "!tset", 5);
	CPPUNIT_ASSERT(c_n5 == 0);
	free(n5);

	bool s2 = bits->seek(10, true);
	CPPUNIT_ASSERT(s2);

	unsigned char *n2 = bits->read(2, true);
	unsigned int c_n2 = memcmp(n2, "si", 2);
	CPPUNIT_ASSERT(c_n2 == 0);
	free(n2);

	CPPUNIT_ASSERT(bits->getPosition() == 7);

	delete bits;
}

void BitsTests::testRead_uint8(){
	unsigned char chunk[] = "Th\0is is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	uint8_t i = bits->read_uint8();
	CPPUNIT_ASSERT(i == 84 && bits->checkIfError() == false);

	i = bits->read_uint8();
	CPPUNIT_ASSERT(i == 104 && bits->checkIfError() == false);

	i = bits->read_uint8();
	CPPUNIT_ASSERT(i == 0 && bits->checkIfError() == false);

	bits->seek(12);

	i = bits->read_uint8();
	CPPUNIT_ASSERT(i == 33 && bits->checkIfError() == false);

	i = bits->read_uint8();
	CPPUNIT_ASSERT(i == 0 && bits->checkIfError() == true);

	delete bits;
}

void BitsTests::testRead_uint16(){
	unsigned char chunk[] = "Th\0\0is is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	uint16_t i = bits->read_uint16();
	CPPUNIT_ASSERT(i == 21608 && bits->checkIfError() == false);

	i = bits->read_uint16();
	CPPUNIT_ASSERT(i == 0 && bits->checkIfError() == false);

	bits->seek(11);

	i = bits->read_uint16();
	CPPUNIT_ASSERT(i == 29729 && bits->checkIfError() == false);

	i = bits->read_uint16();
	CPPUNIT_ASSERT(i == 0 && bits->checkIfError() == true);

	delete bits;
}

void BitsTests::testRead_uint32(){
	unsigned char chunk[] = "This\0\0\0\0 is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	uint32_t i = bits->read_uint32();
	CPPUNIT_ASSERT(i == 1416128883 && bits->checkIfError() == false);

	i = bits->read_uint32();
	CPPUNIT_ASSERT(i == 0 && bits->checkIfError() == false);

	bits->seek(7);

	i = bits->read_uint32();
	CPPUNIT_ASSERT(i == 1702065185 && bits->checkIfError() == false);

	i = bits->read_uint32();
	CPPUNIT_ASSERT(i == 0 && bits->checkIfError() == true);

	delete bits;
}

void BitsTests::testRead_uint64(){
	unsigned char chunk[] = "This is \0\0\0\0\0\0\0\0a very big test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	uint64_t i = bits->read_uint64(); // "This is " == {0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20}
	CPPUNIT_ASSERT( memcmp((unsigned char *)&i, "\x20\x73\x69\x20\x73\x69\x68\x54", 8) == 0 && bits->checkIfError() == false);

	i = bits->read_uint64();
	CPPUNIT_ASSERT(i == 0 && bits->checkIfError() == false);

	bits->seek(8);

	i = bits->read_uint64(); // "ig test!" == {0x69, 0x67, 0x20, 0x74, 0x65, 0x73, 0x74, 0x21}
	CPPUNIT_ASSERT( memcmp((unsigned char *)&i, "\x21\x74\x73\x65\x74\x20\x67\x69", 8) == 0 && bits->checkIfError() == false);

	i = bits->read_uint64();
	CPPUNIT_ASSERT(i == 0 && bits->checkIfError() == true);

	delete bits;
}

void BitsTests::testReadBits(){
	unsigned char chunk[] = "EFGHIJK";
	size_t size = sizeof(chunk) - 1;
	ostringstream out;
	streambuf *orig_buf = cout.rdbuf();

	Bits *bits = new Bits(chunk, size);

	Bits *bits2 = bits->readBits(56);
	cout.rdbuf(out.rdbuf());
	bits2->printAsBinary(1);
	cout.rdbuf(orig_buf);
	unsigned int c1 = memcmp(out.str().c_str(), "01000101", 8);
	CPPUNIT_ASSERT(c1 == 0);
	CPPUNIT_ASSERT(bits->getPosition() == 7);

	out.str("");
	out.clear();
	bits2->setPosition(2);
	cout.rdbuf(out.rdbuf());
	bits2->printAsBinary(1);
	cout.rdbuf(orig_buf);
	unsigned int c2 = memcmp(out.str().c_str(), "01000111", 8);
	CPPUNIT_ASSERT(c2 == 0);
	delete bits2;

	bits->setPosition(0);
	Bits *bits3 = bits->readBits(64);
	CPPUNIT_ASSERT(bits3 == NULL);

	bits->setPosition(0);
	bits3 = bits->readBits(56, 1);
	CPPUNIT_ASSERT(bits3 == NULL);
	delete bits3;

	out.str("");
	out.clear();
	bits->setPosition(0);
	Bits *bits4 = bits->readBits(8, 4);
	cout.rdbuf(out.rdbuf());
	bits4->printAsBinary(1);
	cout.rdbuf(orig_buf);
	unsigned int c3 = memcmp(out.str().c_str(), "01010100", 8);
	CPPUNIT_ASSERT(c3 == 0);
	CPPUNIT_ASSERT(bits->getPosition() == 2);
	delete bits4;

	out.str("");
	out.clear();
	bits->setPosition(0);
	cout.rdbuf(out.rdbuf());
	Bits *bits5 = bits->readBits(48, 3);
	bits5->printAsBinary(6);
	cout.rdbuf(orig_buf);
	unsigned int c4 = memcmp(out.str().c_str(), "00101010 00110010 00111010 01000010 01001010 01010010", 48 + 5);
	CPPUNIT_ASSERT(c4 == 0);
	CPPUNIT_ASSERT(bits->getPosition() == 7);
	delete bits5;

	out.str("");
	out.clear();
	bits->setPosition(0);
	Bits *bits6 = bits->readBits(16, 12);
	cout.rdbuf(out.rdbuf());
	bits6->printAsBinary(2);
	cout.rdbuf(orig_buf);
	unsigned int c5 = memcmp(out.str().c_str(), "01100100 01110100", 16 + 1);
	CPPUNIT_ASSERT(c5 == 0);
	CPPUNIT_ASSERT(bits->getPosition() == 4);
	delete bits6;

	Bits *bits7 = new Bits(chunk, 1);

	out.str("");
	out.clear();
	Bits *bits8 = bits7->readBits(3, 5);
	CPPUNIT_ASSERT(bits8 != NULL);
	cout.rdbuf(out.rdbuf());
	bits8->printAsBinary(1);
	cout.rdbuf(orig_buf);
	unsigned int c6 = memcmp(out.str().c_str(), "10100000", 8);
	CPPUNIT_ASSERT(c6 == 0);
	CPPUNIT_ASSERT(bits7->getPosition() == 1);
	delete bits7;
	delete bits8;

	delete bits;
}

void BitsTests::testCompareBinary(){
	unsigned char chunk[] = "This";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	bool res = bits->compareBinary("01010100", 8);
	CPPUNIT_ASSERT(res);

	bits->setPosition(1);
	bool res2 = bits->compareBinary("000", 3, 5);
	CPPUNIT_ASSERT(res2);

	bits->setPosition(2);
	bool res3 = bits->compareBinary("01101001 01110011", 16);
	CPPUNIT_ASSERT(res3);

	bits->setPosition(0);
	bool res4 = bits->compareBinary("01010100 01101000 01", 18);
	CPPUNIT_ASSERT(res4);

	bool res5 = bits->compareBinary("01010001 10100001 10", 18, 2);
	CPPUNIT_ASSERT(res5);

	bool res6 = bits->compareBinary("2", 1);
	CPPUNIT_ASSERT(res6 == false);

	bool res7 = bits->compareBinary("01", 3);
	CPPUNIT_ASSERT(res7 == false);

	delete bits;
}

void BitsTests::testCompareHex(){
	unsigned char chunk[] = "This";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	bool res = bits->compareHex("54 68 69 73", 4);
	CPPUNIT_ASSERT(res);

	bool res2 = bits->compareHex("546869XX", 3);
	CPPUNIT_ASSERT(res2);

	bool res3 = bits->compareHex("546869XX", 4);
	CPPUNIT_ASSERT(res3 == false);

	bool res4 = bits->compareHex("54", 3);
	CPPUNIT_ASSERT(res4 == false);

	delete bits;
}

void BitsTests::testPeek(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	bool s1 = bits->seek(16);
	CPPUNIT_ASSERT(s1 == false);

	unsigned char *n5 = bits->peek(5, true);
	unsigned int c_n5 = memcmp(n5, " sihT", 5);
	CPPUNIT_ASSERT(c_n5 == 0);
	free(n5);

	unsigned char *n1 = bits->peek(1);
	unsigned char *n2 = bits->peek(1);
	unsigned int c_n1 = memcmp(n1, n2, 1);
	CPPUNIT_ASSERT(c_n1 == 0);
	free(n1);
	free(n2);

	CPPUNIT_ASSERT(bits->getPosition() == 0);

	delete bits;
}

void BitsTests::testWrite(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size, true);
	bits->write((unsigned char *)"That", 4);

	unsigned int c1 = memcmp(bits->getData(), "That is a test!", size);
	CPPUNIT_ASSERT(c1 == 0);

	bits->write((unsigned char *)" string", 7, false);

	unsigned int c2 = memcmp(bits->getData(), "That string is a test!", size + 7);
	CPPUNIT_ASSERT(c2 == 0);

	delete bits;
}

void BitsTests::testSeek(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	bool s1 = bits->seek(6);
	CPPUNIT_ASSERT(s1);

	unsigned char *n3 = bits->read(3);
	unsigned int c_n3 = memcmp(n3, "s a", 3);
	CPPUNIT_ASSERT(c_n3 == 0);
	free(n3);

	bool s2 = bits->seek(6);
	CPPUNIT_ASSERT(s2);

	unsigned char *n1 = bits->read(1);
	CPPUNIT_ASSERT(n1 == NULL);
	free(n1);

	bool s3 = bits->seek(2, true);
	CPPUNIT_ASSERT(s3);

	unsigned char *p2 = bits->read(2);
	unsigned int c_p2 = memcmp(p2, "t!", 2);
	CPPUNIT_ASSERT(c_p2 == 0);
	free(p2);

	bool s4 = bits->seek(15, true);
	CPPUNIT_ASSERT(s4);

	bool s5 = bits->seek(1);
	CPPUNIT_ASSERT(s5);

	bool s6 = bits->seek(2, true);
	CPPUNIT_ASSERT(s6 == false);

	delete bits;
}

void BitsTests::testFindPrevious(){
	unsigned char chunk[] = "This is a really long test with some 28452 random data in 9352 it!!?";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	uint64_t p1 = bits->findNext("!", 1);
	CPPUNIT_ASSERT(p1 == 65);

	bits->setPosition(p1 + 1);

	uint64_t p2 = bits->findNext("!", 1);
	CPPUNIT_ASSERT(p2 == 66);

	bits->setPosition(p2);

	uint64_t p3 = bits->findPrevious("45", 2);
	CPPUNIT_ASSERT(p3 == 39);

	bits->setPosition(bits->getMaxPosition());

	uint64_t p4 = bits->findPrevious("!?", 2);
	CPPUNIT_ASSERT(p4 == 66);

	bits->setPosition(0);

	uint64_t p5 = bits->findPrevious("T", 1);
	CPPUNIT_ASSERT(p5 == 0 && bits->checkIfError() == true);

	delete bits;
}

void BitsTests::testFindNext(){
	unsigned char chunk[] = "This is a really long test with some 28452 random data in 9352 it!!?";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	uint64_t p1 = bits->findNext("a", 1);
	CPPUNIT_ASSERT(p1 == 8);

	uint64_t p2 = bits->findNext("rand", 4);
	CPPUNIT_ASSERT(p2 == 43);

	bits->setPosition(p2);

	uint64_t p3 = bits->findNext("52", 2);
	CPPUNIT_ASSERT(p3 == 60);

	bits->setPosition(bits->getMaxPosition());

	uint64_t p4 = bits->findNext("?", 1);
	CPPUNIT_ASSERT(p4 == 0 && bits->checkIfError() == true);

	delete bits;
}

void BitsTests::testBit(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	bool b1 = bits->testBit(0);
	CPPUNIT_ASSERT(b1 == false);

	bool b2 = bits->testBit(1);
	CPPUNIT_ASSERT(b2 == false);

	bool b3 = bits->testBit(2);
	CPPUNIT_ASSERT(b3);

	bool b4 = bits->testBit(3);
	CPPUNIT_ASSERT(b4 == false);

	bool b5 = bits->testBit(4);
	CPPUNIT_ASSERT(b5);

	bool b6 = bits->testBit(5);
	CPPUNIT_ASSERT(b6 == false);

	bool b7 = bits->testBit(6);
	CPPUNIT_ASSERT(b7);

	bool b8 = bits->testBit(7);
	CPPUNIT_ASSERT(b8 == false);

	delete bits;
}

void BitsTests::testSetBit(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	bits->setBit(0);
	bits->setBit(1);

	unsigned char *c_c1 = bits->read(1);
	unsigned int c1 = memcmp(c_c1, "W", 1);
	CPPUNIT_ASSERT(c1 == 0);
	free(c_c1);

	bits->seek(1);

	bits->setBit(0);
	bits->setBit(1);

	unsigned char *c_c2 = bits->read(1);
	unsigned int c2 = memcmp(c_c2, "k", 1);
	CPPUNIT_ASSERT(c2 == 0);
	free(c_c2);

	delete bits;
}

void BitsTests::testUnsetBit(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	bits->unsetBit(4);

	unsigned char *c_c1 = bits->read(1);
	unsigned int c1 = memcmp(c_c1, "D", 1);
	CPPUNIT_ASSERT(c1 == 0);
	free(c_c1);

	bits->seek(1);

	bits->unsetBit(5);

	unsigned char *c_c2 = bits->read(1);
	unsigned int c2 = memcmp(c_c2, "I", 1);
	CPPUNIT_ASSERT(c2 == 0);
	free(c_c2);

	delete bits;
}

void BitsTests::testToggleBit(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	bits->toggleBit(0);
	bits->toggleBit(1);

	unsigned char *c_c1 = bits->read(1);
	unsigned int c1 = memcmp(c_c1, "W", 1);
	CPPUNIT_ASSERT(c1 == 0);
	free(c_c1);

	bits->seek(7);

	bits->unsetBit(0);

	unsigned char *c_c2 = bits->read(1);
	unsigned int c2 = memcmp(c_c2, "`", 1);
	CPPUNIT_ASSERT(c2 == 0);
	free(c_c2);

	delete bits;
}

void BitsTests::testPrintHash(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	//Capture cout
	ostringstream out;
	streambuf *orig_buf(cout.rdbuf(out.rdbuf()));

	bits->printHash();

	//Restore cout
	cout.rdbuf(orig_buf);

	unsigned int c1 = memcmp(out.str().c_str(), "706f2f62", sizeof(uint32_t));
	CPPUNIT_ASSERT(c1 == 0);

	delete bits;
}

void BitsTests::testGetAsHex(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	unsigned char *s = bits->getAsHex(size);
	unsigned int c1 = memcmp(s, "546869732069732061207465737421", (size * 2));
	CPPUNIT_ASSERT(c1 == 0);

	free(s);
	delete bits;
}

void BitsTests::testPrintAsHex(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	//Capture cout
	ostringstream out;
	streambuf *orig_buf(cout.rdbuf(out.rdbuf()));

	bits->printAsHex(size);

	//Restore cout
	cout.rdbuf(orig_buf);

	unsigned int c1 = memcmp(out.str().c_str(), "54 68 69 73 20 69 73 20 61 20 74 65 73 74 21", (size * 2) + (size - 1));
	CPPUNIT_ASSERT(c1 == 0);

	delete bits;
}

void BitsTests::testGetAsBinary(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	unsigned char *s = bits->getAsBinary(2);
	unsigned int c1 = memcmp(s, "0101010001101000", 16);
	CPPUNIT_ASSERT(c1 == 0);

	free(s);
	delete bits;
}

void BitsTests::testPrintAsBinary(){
	unsigned char chunk[] = "This is a test!";
	size_t size = sizeof(chunk) - 1;

	Bits *bits = new Bits(chunk, size);

	//Capture cout
	ostringstream out;
	streambuf *orig_buf(cout.rdbuf(out.rdbuf()));

	bits->printAsBinary(2);

	//Restore cout
	cout.rdbuf(orig_buf);

	unsigned int c1 = memcmp(out.str().c_str(), "01010100 01101000", 16);
	CPPUNIT_ASSERT(c1 == 0);

	delete bits;
}
