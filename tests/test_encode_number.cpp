#include <vector>
#include <iostream>

#include <hybridspi/binary.h>
#include <hybridspi/util.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TextTestRunner.h>

using namespace hybridspi;
using namespace hybridspi::binary;

class EncodeNumberTest : public CppUnit::TestFixture 
{

    CPPUNIT_TEST_SUITE(EncodeNumberTest);
    CPPUNIT_TEST(test8BitInt);
    CPPUNIT_TEST(test16BitInt);
    CPPUNIT_TEST(test24BitInt);
    CPPUNIT_TEST_SUITE_END();

    void test8BitInt()
    {
        CPPUNIT_ASSERT(bytes_to_hex(encode_number<8>(18)) == "12");
    }

    void test16BitInt()
    {
        CPPUNIT_ASSERT(bytes_to_hex(encode_number<16>(18)) == "00 12");
        CPPUNIT_ASSERT(bytes_to_hex(encode_number<16>(30000)) == "75 30");        
    }

    void test24BitInt()
    {
        CPPUNIT_ASSERT(bytes_to_hex(encode_number<24>(14000000)) == "D5 9F 80");
    }
    
};

int main()
{
    CppUnit::TextTestRunner runner;
    runner.addTest(EncodeNumberTest::suite());
    return (runner.run() ? 0 : 1);
}