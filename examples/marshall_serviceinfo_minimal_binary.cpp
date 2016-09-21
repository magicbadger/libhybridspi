#include <hybridspi/services.h>
#include <hybridspi/binary.h>
#include <hybridspi/util.h>

#include "create_serviceinfo_minimal.h"

#include <iostream>

using namespace hybridspi;
using namespace hybridspi::binary;

int main()
{
    ServiceInfo info = create_serviceinfo();

    // ensemble - just for binary
    Ensemble ensemble(0xe1, 0xc181);

    BinaryMarshaller marshaller(ensemble);
    vector<unsigned char> bytes = marshaller.Marshall(info);

    cout << bytes;
}
