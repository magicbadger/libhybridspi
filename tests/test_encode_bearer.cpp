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

class EncodeBearerTest : public CppUnit::TestFixture 
{

    CPPUNIT_TEST_SUITE(EncodeBearerTest);
    CPPUNIT_TEST_EXCEPTION(testFmBearer, unable_to_encode_bearer);
    CPPUNIT_TEST(testDabBearer);
    CPPUNIT_TEST(testIpBearer);
    CPPUNIT_TEST_SUITE_END();

    public:

        void testFmBearer()
        {
            FmBearer bearer(0xe1, 0xc479, 95800);
            Element e = build_bearer(&bearer);
            bytes_to_hex(e.encode());
        }

        void testDabBearer()
        {
            DabBearer bearer(0xe1, 0xcfff, 0xcff1, 0, "audio/mpeg");
            bearer.SetBitrate(128000);
            Element e = build_bearer(&bearer);
            CPPUNIT_ASSERT(bytes_to_hex(e.encode()) == "29 08 80 06 40 E1 CF FF CF F1");        
        }

        void testIpBearer()
        {
            IpBearer bearer("http://my.test.domain/testing.pls", "audio/mpeg");
            Element e = build_bearer(&bearer);
            CPPUNIT_ASSERT(bytes_to_hex(e.encode()) == "29 23 82 21 68 74 74 70 3A 2F 2F 6D 79 2E 74 65 73 74 2E 64 6F 6D 61 69 6E 2F 74 65 73 74 69 6E 67 2E 70 6C 73"); 
        }
};

int main()
{
    CppUnit::TextTestRunner runner;
    runner.addTest(EncodeBearerTest::suite());
    return (runner.run() ? 0 : 1);
}
