#include <vector>
#include <iostream>
#include <time.h>
#include <sstream>
#include <iomanip>

#include <hybridspi/mot.h>
#include <hybridspi/util.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TextTestRunner.h>

using namespace hybridspi::mot;

class EncodeHeaderParametersTest : public CppUnit::TestFixture 
{

    CPPUNIT_TEST_SUITE(EncodeHeaderParametersTest);   
    CPPUNIT_TEST(testScopeId);
    CPPUNIT_TEST(testScopeStart);
    CPPUNIT_TEST(testScopeEnd);
    CPPUNIT_TEST_SUITE_END();

    void testScopeId()
    {
        ScopeId parameter(0xe1, 0xc479);
        vector<unsigned char> bytes = parameter.Encode(); 
        CPPUNIT_ASSERT(bytes_to_hex(parameter.Encode()) == "E7 03 E1 C4 79");

        parameter = ScopeId(0xd0, 0xefff);
        CPPUNIT_ASSERT(bytes_to_hex(parameter.Encode()) == "E7 03 D0 EF FF");
    }

    void testScopeStart()
    {
        tm t = {};
        istringstream ss("2015-09-21T06:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        time_t epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);
        ScopeStart parameter(tp);
        CPPUNIT_ASSERT(bytes_to_hex(parameter.Encode()) == "E5 06 37 F1 89 9E 3C 00");
    }

    void testScopeEnd()
    {
        tm t = {};
        istringstream ss("2015-09-21T07:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        time_t epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);
        ScopeEnd parameter(tp);
        CPPUNIT_ASSERT(bytes_to_hex(parameter.Encode()) == "E6 06 37 F1 89 DE 3C 00");
    }

};

int main()
{
    CppUnit::TextTestRunner runner;
    runner.addTest(EncodeHeaderParametersTest::suite());
    return (runner.run() ? 0 : 1);
}