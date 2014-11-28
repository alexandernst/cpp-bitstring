#ifndef BITSTESTS_H
#define	BITSTESTS_H

#include <cppunit/extensions/HelperMacros.h>

class BitsTests : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(BitsTests);

	CPPUNIT_TEST(testFromMem);
	CPPUNIT_TEST(testUnload);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST(testPeek);
	CPPUNIT_TEST(testWrite);
	CPPUNIT_TEST(testSeek);

	CPPUNIT_TEST(testFindPrevious);
	CPPUNIT_TEST(testFindNext);

	CPPUNIT_TEST(testBit);
	CPPUNIT_TEST(testSetBit);
	CPPUNIT_TEST(testUnsetBit);
	CPPUNIT_TEST(testToggleBit);

	CPPUNIT_TEST(testPrintHex);
	CPPUNIT_TEST(testPrintBits);

	CPPUNIT_TEST_SUITE_END();

public:
	BitsTests();
	virtual ~BitsTests();
	void setUp();
	void tearDown();

private:
	void testFromMem();
	void testUnload();
	void testRead();
	void testPeek();
	void testWrite();
	void testSeek();

	void testFindPrevious();
	void testFindNext();

	void testBit();
	void testSetBit();
	void testUnsetBit();
	void testToggleBit();

	void testPrintHex();
	void testPrintBits();

};

#endif	/* BITSTESTS_H */
