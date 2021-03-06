#ifndef BITSTESTS_H
#define	BITSTESTS_H

#include <cppunit/extensions/HelperMacros.h>

class BitsTests : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(BitsTests);

	CPPUNIT_TEST(testCanMoveBackwards);
	CPPUNIT_TEST(testCanMoveForward);

	CPPUNIT_TEST(testFromFile);
	CPPUNIT_TEST(testToFile);
	CPPUNIT_TEST(testFromMem);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST(testRead_uint8);
	CPPUNIT_TEST(testRead_uint16);
	CPPUNIT_TEST(testRead_uint32);
	CPPUNIT_TEST(testRead_uint64);
	CPPUNIT_TEST(testReadBits);
	CPPUNIT_TEST(testCompareBinary);
	CPPUNIT_TEST(testCompareHex);
	CPPUNIT_TEST(testPeek);
	CPPUNIT_TEST(testWrite);
	CPPUNIT_TEST(testSeek);

	CPPUNIT_TEST(testFindPrevious);
	CPPUNIT_TEST(testFindNext);

	CPPUNIT_TEST(testBit);
	CPPUNIT_TEST(testSetBit);
	CPPUNIT_TEST(testUnsetBit);
	CPPUNIT_TEST(testToggleBit);

	CPPUNIT_TEST(testPrintHash);

	CPPUNIT_TEST(testGetAsHex);
	CPPUNIT_TEST(testPrintAsHex);

	CPPUNIT_TEST(testGetAsBinary);
	CPPUNIT_TEST(testPrintAsBinary);

	CPPUNIT_TEST_SUITE_END();

	public:
		BitsTests();
		virtual ~BitsTests();
		void setUp();
		void tearDown();

	private:
		void testCanMoveBackwards();
		void testCanMoveForward();
		void testFromFile();
		void testToFile();
		void testFromMem();
		void testRead();
		void testRead_uint8();
		void testRead_uint16();
		void testRead_uint32();
		void testRead_uint64();
		void testReadBits();
		void testCompareBinary();
		void testCompareHex();
		void testPeek();
		void testWrite();
		void testSeek();

		void testFindPrevious();
		void testFindNext();

		void testBit();
		void testSetBit();
		void testUnsetBit();
		void testToggleBit();

		void testPrintHash();

		void testGetAsHex();
		void testPrintAsHex();

		void testGetAsBinary();
		void testPrintAsBinary();

};

#endif	/* BITSTESTS_H */
