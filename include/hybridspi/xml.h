#ifndef HYBRIDSPI_XML_H
#define HYBRIDSPI_XML_H

#include <hybridspi/marshall.h>
#include <vector>
#include <tinyxml2.h>

using namespace std;
using namespace hybridspi;
using namespace tinyxml2;

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
            
                XMLElement* build_service(XMLDocument *doc, Service &service) const;
                
                XMLElement* build_name(XMLDocument* doc, Name &name) const;

                XMLElement* build_description(XMLDocument* doc, Description &description) const;

                XMLElement* build_media(XMLDocument* doc, Multimedia &media) const;

                XMLElement* build_genre(XMLDocument* doc, Genre &genre) const;

                XMLElement* build_keywords(XMLDocument* doc, vector<string> &keywords) const;

                XMLElement* build_link(XMLDocument* doc, Link &link) const;
        };        
    }
}

#endif