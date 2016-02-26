#ifndef HYBRIDSPI_XML_H
#define HYBRIDSPI_XML_H

#include <hybridspi/marshall.h>
#include <vector>

using namespace std;
using namespace hybridspi;

namespace hybridspi
{
    namespace xml
    {
        class XmlMarshaller: public Marshaller     
        {
            public:
            
                XmlMarshaller();
            
                vector<unsigned char> Marshall(ServiceInfo service_info) const;
                
            private:
            
        };        
    }
}

#endif