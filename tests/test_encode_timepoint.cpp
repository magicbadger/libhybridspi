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

class EncodeTimepointTest : public CppUnit::TestFixture 
{

    CPPUNIT_TEST_SUITE(EncodeTimepointTest);   
    CPPUNIT_TEST(testTimepointToMjd);
    CPPUNIT_TEST(testEncodedTimepointLong);
    CPPUNIT_TEST(testEncodedTimepointShort);
    CPPUNIT_TEST(testEncodedTimepointShortAttribute);
    CPPUNIT_TEST_SUITE_END();

    void testTimepointToMjd()
    {
        tm t = {};
        stringstream ss("2015-09-21T06:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        auto epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);
                
        int mjd = timepoint_to_mjd(tp);
        CPPUNIT_ASSERT(mjd == 57286);
    }
    
    void testEncodedTimepointLong()
    {
        tm t = {};
        stringstream ss("2015-09-21T06:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        auto epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);        

        CPPUNIT_ASSERT(bytes_to_hex(encode_timepoint(tp)) == "37 F1 89 9E 3C 00");
    }
    
    void testEncodedTimepointShort()
    {
        tm t = {};
        stringstream ss("2016-02-29T15:11:00");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        auto epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);        

        CPPUNIT_ASSERT(bytes_to_hex(encode_timepoint(tp)) == "38 19 C3 CB");
    }
    
    void testEncodedTimepointShortAttribute()
    {
        tm t = {};
        stringstream ss("2016-02-29T15:11:00");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        auto epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);  
        Attribute a(0x81, encode_timepoint(tp));              

        CPPUNIT_ASSERT(bytes_to_hex(a.encode()) == "81 04 38 19 C3 CB");        
    }
};

int main()
{
    CppUnit::TextTestRunner runner;
    runner.addTest(EncodeTimepointTest::suite());
    return (runner.run() ? 0 : 1);
}
