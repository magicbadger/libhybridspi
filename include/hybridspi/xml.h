#ifndef HYBRIDSPI_XML_H
#define HYBRIDSPI_XML_H

#include <hybridspi/marshall.h>
#include <hybridspi/common.h>
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
                
                XMLElement* build_schedule(XMLDocument* doc, Schedule &schedule) const;
                
                XMLElement* build_programme(XMLDocument* doc, Programme &programme) const;

                XMLElement* build_event(XMLDocument* doc, ProgrammeEvent &event) const;                
                
                XMLElement* build_membership(XMLDocument* doc, Membership &membership) const;
                
                XMLElement* build_location(XMLDocument* doc, Location &location) const;
                
                XMLElement* build_bearer(XMLDocument* doc, Bearer *bearer) const;
                               
                string build_datetime(const DateTime &datetime) const;
                
                string build_duration(const Duration &duration) const;
                
                // unmarshalling
                Service parse_service(XMLElement* serviceElement) const;
                
                Name parse_name(XMLElement* element) const;   
                
                Description parse_description(XMLElement* element) const;

                Schedule parse_schedule(XMLElement* element) const;

                Programme parse_programme(XMLElement* element) const;

                Location parse_location(XMLElement* locationElement) const;
                                             
        };        

        DateTime parse_datetime(string input);

        Duration parse_duration(string input);

        // exceptions

        class invalid_duration: public exception
        {

            std::string duration_string;

            public: 

            invalid_duration(const string duration_string) 
                : duration_string(duration_string)
            {}

            virtual const char* what() const throw()
            {
                return ("invalid duration string: " + duration_string).c_str();
            }
        };
    }
}

#endif
