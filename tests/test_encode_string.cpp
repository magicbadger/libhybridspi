#include <hybridspi/binary.h>
#include <hybridspi/util.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TextTestRunner.h>

using namespace hybridspi;
using namespace hybridspi::binary;

class EncodeStringTest : public CppUnit::TestFixture 
{

    CPPUNIT_TEST_SUITE(EncodeStringTest);   
    CPPUNIT_TEST(testEncodeString);
    CPPUNIT_TEST_SUITE_END();

    void testEncodeString()
    {
        CPPUNIT_ASSERT(bytes_to_hex(encode_string("testing")) == "74 65 73 74 69 6E 67");
    }
 
};

int main()
{
    CppUnit::TextTestRunner runner;
    runner.addTest(EncodeStringTest::suite());
    return (runner.run() ? 0 : 1);
}
