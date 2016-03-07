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
        try
        {
            FmBearer bearer(0xe1, 0xc479, 95800);
            Element e = build_bearer(&bearer);
            bytes_to_hex(e.encode());
            ASSERT("encoded FM bearer", 0, 1);
        }
        catch(exception& e)
        {
            ASSERT("encoded FM bearer", 0, 0);
        }
    }
    
    {
        DabBearer bearer(0xe1, 0xcfff, 0xcff1, 0, 128000, "audio/mpeg");
        ASSERT("encoded DAB bearer", bytes_to_hex(build_bearer(&bearer).encode()), "29 08 80 06 40 E1 CF FF CF F1");        
    }

    return 0;
}
