#define CATCH_CONFIG_MAIN

#include <vector>
#include <iostream>
#include <time.h>

#include <mot.h> 
#include <hybridspi_mot.h>

#include "catch.hpp"

using namespace std;
using namespace spi;

TEST_CASE( "SPI MOT parameter encoding tests", "[spi]" ) {

	SECTION ("SCOPEID") 
    {
        ScopeId parameter(0xe1, 0xc479);
        vector<unsigned char> bytes = parameter.encode(); 
		REQUIRE(bytes_to_hex(bytes) == "E7 03 E1 C4 79");

        parameter = ScopeId(0xd0, 0xefff);
        bytes = parameter.encode(); 
		REQUIRE(bytes_to_hex(bytes) == "E7 03 D0 EF FF");
    }

    SECTION ("SCOPESTART") 
    {
        tm t = {};
        istringstream ss("2015-09-21T06:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        time_t epoch = mktime(&t);
        ScopeStart parameter(epoch);
        vector<unsigned char> bytes = parameter.encode();
        REQUIRE(bytes_to_hex(bytes) == "E5 06 B7 F1 89 9E 3C 00");
    }

    SECTION ("SCOPEEND") 
    {
        tm t = {};
        istringstream ss("2015-09-21T07:30:15");
        ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
        time_t epoch = mktime(&t);
        ScopeEnd parameter(epoch);
        vector<unsigned char> bytes = parameter.encode();
        REQUIRE(bytes_to_hex(bytes) == "E6 06 B7 F1 89 DE 3C 00");
    }


}
