#include <hybridspi/xml.h>
#include <hybridspi/util.h>
#include <iostream>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TextTestRunner.h>

using namespace hybridspi;
using namespace hybridspi::xml;

using namespace chrono;

class ParseDurationTest : public CppUnit::TestFixture 
{

    CPPUNIT_TEST_SUITE(ParseDurationTest);   
    CPPUNIT_TEST(testParseDuration1);
    CPPUNIT_TEST(testParseDuration2);
    CPPUNIT_TEST_SUITE_END();

    void testParseDuration1()
    {
        string duration(string("PT2H30M25S"));
        CPPUNIT_ASSERT(parse_duration(duration.c_str()) == seconds(9025)); 
    }

    void testParseDuration2()
    {
        string duration(string("PT1400S"));
        CPPUNIT_ASSERT(parse_duration(duration.c_str()) == seconds(1400));
    }
 
};

int main()
{
    CppUnit::TextTestRunner runner;
    runner.addTest(ParseDurationTest::suite());
    return (runner.run() ? 0 : 1);
}
