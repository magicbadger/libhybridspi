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
                                
                vector<unsigned char> Marshall(ProgrammeInfo programme_info) const;
            
                vector<unsigned char> Marshall(GroupInfo group_info) const;
            
                ServiceInfo UnmarshallServiceInfo(vector<unsigned char> bytes) const;
            
                ProgrammeInfo UnmarshallProgrammeInfo(vector<unsigned char> bytes) const;
            
                GroupInfo UnmarshallGroupInfo(vector<unsigned char> bytes) const;                
                
            private:
            
                // marshalling
                XMLElement* build_service(XMLDocument *doc, Service &service) const;
                
                XMLElement* build_name(XMLDocument* doc, Name &name) const;

                XMLElement* build_description(XMLDocument* doc, Description &description) const;

                XMLElement* build_media(XMLDocument* doc, Multimedia &media) const;

                XMLElement* build_genre(XMLDocument* doc, Genre &genre) const;

                XMLElement* build_keywords(XMLDocument* doc, vector<string> &keywords) const;

                XMLElement* build_link(XMLDocument* doc, Link &link) const;
                
                // unmarshalling
                Service parse_service(XMLElement* serviceElement) const;
                
                Name parse_name(XMLElement* element) const;   
                
                Description parse_description(XMLElement* element) const;
                                             
        };        
    }
}

#endif