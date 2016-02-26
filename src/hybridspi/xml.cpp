#include <hybridspi/xml.h>

namespace hybridspi
{
    namespace xml
    {
        XmlMarshaller::XmlMarshaller()
            : Marshaller()
        { }

        vector<unsigned char> XmlMarshaller::Marshall(ServiceInfo service_info) const
        {
            vector<unsigned char> bytes;
            return bytes;
        }
    }
}