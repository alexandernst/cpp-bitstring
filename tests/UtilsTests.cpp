#include "UtilsTests.h"
#include "src/Utils.h"
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(UtilsTests);

UtilsTests::UtilsTests(){
}

UtilsTests::~UtilsTests(){
}

void UtilsTests::setUp(){
    //Starting tests...
}

void UtilsTests::tearDown(){
    //Finishing tests...
}

void UtilsTests::testIsValidHexString(){
    bool b = Utils::isValidHexString("ABCDEF01");

    CPPUNIT_ASSERT(b == true);

    b = Utils::isValidHexString("XE");

    CPPUNIT_ASSERT(b == false);
}

void UtilsTests::testIsValidBinString(){
    bool b = Utils::isValidBinString("01011001");

    CPPUNIT_ASSERT(b == true);

    b = Utils::isValidBinString("02");

    CPPUNIT_ASSERT(b == false);
}

void UtilsTests::testRemoveSpaces(){
    string s = Utils::removeSpaces("abc");

    CPPUNIT_ASSERT(s.compare("abc") == 0);

    s = Utils::removeSpaces(" a b c ");

    CPPUNIT_ASSERT(s.compare("abc") == 0);
}

void UtilsTests::testTrim(){
    string s = Utils::trim("\r\na\rb\n\rc\r\n");

    CPPUNIT_ASSERT(s.compare("abc") == 0);
}
