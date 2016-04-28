#include <hybridspi/binary.h>
#include <hybridspi/util.h>

#include "test_util.h"

using namespace std;
using namespace hybridspi;
using namespace hybridspi::binary;

int main()
{
    {
        ASSERT("encoded string", bytes_to_hex(encode_string("testing")), "74 65 73 74 69 6E 67");
    }
    
    return 0;
}
