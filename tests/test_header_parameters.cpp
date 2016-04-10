#include <vector>
#include <iostream>
#include <time.h>
#include <sstream>
#include <iomanip>

#include <mot.h> 
#include <hybridspi/mot.h>
#include <hybridspi/util.h>

#include "test_util.h"

using namespace std;
using namespace hybridspi;

int main()
{
    {
        ScopeId parameter(0xe1, 0xc479);
        vector<unsigned char> bytes = parameter.Encode(); 
        ASSERT("scopeid 1", bytes_to_hex(parameter.Encode()), "E7 03 E1 C4 79");

        parameter = ScopeId(0xd0, 0xefff);
        ASSERT("scopeid 2", bytes_to_hex(parameter.Encode()), "E7 03 D0 EF FF");
    }

    {
        tm t = {};
        istringstream ss("2015-09-21T06:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        time_t epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);
        ScopeStart parameter(tp);
        ASSERT("scope start", bytes_to_hex(parameter.Encode()), "E5 06 37 F1 89 9E 3C 00");
    }

    {
        tm t = {};
        istringstream ss("2015-09-21T07:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        time_t epoch = mktime(&t);
        auto tp = std::chrono::system_clock::from_time_t(epoch);
        ScopeEnd parameter(tp);
        ASSERT("scope end", bytes_to_hex(parameter.Encode()), "E6 06 37 F1 89 DE 3C 00");
    }

    return 0;

}
