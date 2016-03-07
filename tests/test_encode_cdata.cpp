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
        CData data("testing");

        ASSERT("encoded cdata short", bytes_to_hex(data.encode()), "01 07 74 65 73 74 69 6E 67");
    }

    return 0;
}
