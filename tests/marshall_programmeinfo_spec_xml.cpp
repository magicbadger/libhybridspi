#include <hybridspi/services.h>
#include <hybridspi/xml.h>
#include <hybridspi/util.h>
#include <iostream>

#include "create_programmeinfo_spec.h"

using namespace std;
using namespace hybridspi;
using namespace hybridspi::xml;

int main()
{
    ProgrammeInfo info = create_programmeinfo();

    XmlMarshaller marshaller;
    cout << marshaller.Marshall(info) << endl;

    return 0;
}
