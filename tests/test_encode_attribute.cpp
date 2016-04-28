#include <vector>
#include <iostream>
#include <chrono>
#include <time.h>
#include <sstream>

#include <hybridspi/binary.h>
#include <hybridspi/util.h>

#include "test_util.h"

using namespace std;
using namespace hybridspi;
using namespace hybridspi::binary;

int main()
{
    {
        Attribute attribute(0x10, "testing");
        ASSERT("encoded short string attribute", 
            bytes_to_hex(attribute.encode()), "10 07 74 65 73 74 69 6E 67");
    }

    {
        Attribute attribute(0x10, encode_number<8>(1));
        ASSERT("encoded short int attribute", 
            bytes_to_hex(attribute.encode()), "10 01 01");
    }

    return 0;
}
