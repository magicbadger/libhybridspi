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

class EncodeNameTest : public CppUnit::TestFixture 
{

    CPPUNIT_TEST_SUITE(EncodeNameTest);
    CPPUNIT_TEST(testEncodeShortName);
    CPPUNIT_TEST_SUITE_END();
    
    void testEncodeShortName()
    {
        ShortName name("Heart");
        Element e = build_name(name);
        CPPUNIT_ASSERT(bytes_to_hex(e.encode()) == "10 07 01 05 48 65 61 72 74");
    }

};

int main()
{
    CppUnit::TextTestRunner runner;
    runner.addTest(EncodeNameTest::suite());
    return (runner.run() ? 0 : 1);
}
