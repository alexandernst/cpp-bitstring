#ifndef UTILSTESTS_H
#define UTILSTESTS_H

#include <cppunit/extensions/HelperMacros.h>

class UtilsTests : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(UtilsTests);

	CPPUNIT_TEST(testIsValidHexString);
	CPPUNIT_TEST(testIsValidBinString);
	CPPUNIT_TEST(testRemoveSpaces);
	CPPUNIT_TEST(testTrim);

	CPPUNIT_TEST_SUITE_END();

	public:
		UtilsTests();
		virtual ~UtilsTests();
		void setUp();
		void tearDown();

	private:
		void testIsValidHexString();
		void testIsValidBinString();
		void testRemoveSpaces();
		void testTrim();

};

#endif /* UTILSTESTS_H */
