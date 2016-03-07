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
        ShortName name("Heart");
        Element e = build_name(name);
        ASSERT("encoded short name", bytes_to_hex(e.encode()), "10 07 01 05 48 65 61 72 74");
    }

    return 0;
}
