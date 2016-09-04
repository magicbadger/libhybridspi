#include <vector>
#include <iostream>
#include <chrono>
#include <time.h>
#include <sstream>

#include <hybridspi/binary.h>
#include <hybridspi/util.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TextTestRunner.h>

using namespace hybridspi;
using namespace hybridspi::binary;

class EncodeAttributeTest : public CppUnit::TestFixture 
{

    CPPUNIT_TEST_SUITE(EncodeAttributeTest);
    CPPUNIT_TEST(testEncodeShortString);
    CPPUNIT_TEST(testEncodeShortInt);
    CPPUNIT_TEST_SUITE_END();

    public:

    void testEncodeShortString()
    {
        Attribute attribute(0x10, "testing");
        CPPUNIT_ASSERT(bytes_to_hex(attribute.encode()) == "10 07 74 65 73 74 69 6E 67");
    }

    void testEncodeShortInt()
    {
        Attribute attribute(0x10, encode_number<8>(1));
        CPPUNIT_ASSERT(bytes_to_hex(attribute.encode()) == "10 01 01");
    }

};

int main()
{
    CppUnit::TextTestRunner runner;
    runner.addTest(EncodeAttributeTest::suite());
    return (runner.run() ? 0 : 1);
}
