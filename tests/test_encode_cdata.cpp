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

class EncodeCdataTest : public CppUnit::TestFixture 
{

    CPPUNIT_TEST_SUITE(EncodeCdataTest);
    CPPUNIT_TEST(testCdataShort);
    CPPUNIT_TEST_SUITE_END();
    
    void testCdataShort()
    {
        CData data("testing");
        CPPUNIT_ASSERT(bytes_to_hex(data.encode()) == "01 07 74 65 73 74 69 6E 67");
    }
};

int main()
{
    CppUnit::TextTestRunner runner;
    runner.addTest(EncodeCdataTest::suite());
    return (runner.run() ? 0 : 1);
}
