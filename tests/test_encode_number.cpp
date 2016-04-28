#include <vector>
#include <iostream>

#include <hybridspi/binary.h>
#include <hybridspi/util.h>

#include "test_util.h"

using namespace hybridspi;
using namespace hybridspi::binary;

int main()
{
    ASSERT("encoded 8-bit int", bytes_to_hex(encode_number<8>(18)), "12");
    ASSERT("encoded 16-bit int 1", bytes_to_hex(encode_number<16>(18)), "00 12");
    ASSERT("encoded 16-bit int 2", bytes_to_hex(encode_number<16>(30000)), "75 30");
    ASSERT("encoded 24-bit int", bytes_to_hex(encode_number<24>(14000000)), "D5 9F 80");
    
    return 0;
}
