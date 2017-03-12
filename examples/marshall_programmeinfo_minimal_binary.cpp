#include <hybridspi/services.h>
#include <hybridspi/binary.h>
#include <hybridspi/util.h>
#include <iostream>

#include "create_programmeinfo_minimal.h"

using namespace std;
using namespace hybridspi;
using namespace hybridspi::binary;

int main()
{
    ProgrammeInfo info = create_programmeinfo();

    // ensemble - just for binary
    Ensemble ensemble(0xe1, 0xc181);

    BinaryMarshaller marshaller(ensemble);
    cout << marshaller.Marshall(info) << endl;

    return 0;
}
