#include <vector>
#include <iostream>
#include <time.h>

#include <mot.h> 
#include <hybridspi_mot.h>

#include "test_util.h"

using namespace std;
using namespace hybridspi;

    {
        ScopeId parameter(0xe1, 0xc479);
        vector<unsigned char> bytes = parameter.encode(); 
        ASSERT("scopeid 1", bytes_to_hex(parameter.encode()), "E7 03 E1 C4 79");

        parameter = ScopeId(0xd0, 0xefff);
        ASSERT("scopeid 2", bytes_to_hex(parameter.encode()), "E7 03 D0 EF FF");
    }

    {
        tm t = {};
        istringstream ss("2015-09-21T06:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        time_t epoch = mktime(&t);
        ScopeStart parameter(epoch);
        ASSERT("scope start", bytes_to_hex(parameter.encode()), "E5 06 B7 F1 89 9E 3C 00");
    }

    {
        tm t = {};
        istringstream ss("2015-09-21T07:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        time_t epoch = mktime(&t);
        ScopeEnd parameter(epoch);
        ASSERT("scope end", bytes_to_hex(parameter.encode()), "E6 06 B7 F1 89 DE 3C 00");
    }

}
