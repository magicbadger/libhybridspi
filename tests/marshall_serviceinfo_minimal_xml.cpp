#include <hybridspi/services.h>
#include <hybridspi/xml.h>
#include <hybridspi/util.h>
#include <iostream>

#include "create_serviceinfo_minimal.h"

using namespace std;
using namespace hybridspi;
using namespace hybridspi::xml;

int main()
{
    ServiceInfo info = create_serviceinfo();

    XmlMarshaller marshaller;
    cout << marshaller.Marshall(info) << endl;

    return 0;
}
