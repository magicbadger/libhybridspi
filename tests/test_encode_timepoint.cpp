#include <vector>
#include <iostream>
#include <chrono>
#include <time.h>
#include <sstream>

#include <hybridspi/binary.h>
#include <hybridspi/util.h>

#include "test_util.h"

using namespace std;
using namespace std::chrono;
using namespace hybridspi;
using namespace hybridspi::binary;

int main()
{
    {
        tm t = {};
        stringstream ss("2015-09-21T06:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        auto epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);
                
        int mjd = timepoint_to_mjd(tp);
        ASSERT("timepoint to MJD", mjd, 57286);
    }
    
    {
        tm t = {};
        stringstream ss("2015-09-21T06:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        auto epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);        

        ASSERT("encoded timepoint long", bytes_to_hex(encode_timepoint(tp)), "37 F1 89 9E 3C 00");
    }
    
    {
        tm t = {};
        stringstream ss("2016-02-29T15:11:00");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        auto epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);        

        ASSERT("encoded timepoint short", bytes_to_hex(encode_timepoint(tp)), "38 19 C3 CB");
    }
    
    {
        tm t = {};
        stringstream ss("2016-02-29T15:11:00");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        auto epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);  
        Attribute a(0x81, encode_timepoint(tp));              

        ASSERT("encoded timepoint short attribute", bytes_to_hex(a.encode()), "81 04 38 19 C3 CB");        
        

    }

    return 0;
}
