#include <hybridspi/xml.h>
#include <sstream>
#include <iostream>

using namespace std;
using namespace tinyxml2;

namespace hybridspi
{
    namespace xml
    {
        XmlMarshaller::XmlMarshaller()
            : Marshaller()
        { } 

        /**
         * Marshalling
         */
        vector<unsigned char> XmlMarshaller::Marshall(ServiceInfo info) const
        {         
            
            XMLDocument* doc = new XMLDocument();
            XMLElement* serviceInformationElement = doc->InsertEndChild(doc->NewElement("serviceInformation"))->ToElement();
		    serviceInformationElement->SetAttribute("xmlns", "http://www.worlddab.org/schemas/spi/31");
            serviceInformationElement->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
            serviceInformationElement->SetAttribute("xmlns:xml", "http://www.w3.org/XML/1998/namespace");
            serviceInformationElement->SetAttribute("xsi:schemaLocation", "http://www.worlddab.org/schemas/spi/31 spi_31.xsd");
            
            // TODO print out timezone, rather than relying on local time
            time_t now_c = chrono::system_clock::to_time_t(info.Created());
            stringstream ss;
            ss << put_time(localtime(&now_c), "%Y-%m-%dT%H:%M:%S");
            auto sss = ss.str();
            const char* date_string = sss.c_str(); 
            
            serviceInformationElement->SetAttribute("creationTime", date_string);
            
            // originator
            if(!info.Originator().empty())
            {
                serviceInformationElement->SetAttribute("originator", info.Originator().c_str());
            }
            
            // version
            if(info.Version() > 0)
            {
                serviceInformationElement->SetAttribute("version", info.Version());
            }         
            
            XMLNode* servicesElement = serviceInformationElement->InsertEndChild(doc->NewElement("services"));
            for(auto &service : info.Services())
            {
                servicesElement->InsertEndChild(build_service(doc, service));
            }   
            
            // stream to byte vector
            XMLPrinter printer;  
            doc->Accept(&printer);
            const char *s = printer.CStr();
            vector<unsigned char> bytes(s, s + strlen(s));
            return bytes;
        }
        
        vector<unsigned char> XmlMarshaller::Marshall(ProgrammeInfo programme_info) const
        {            
            XMLDocument* doc = new XMLDocument();
            XMLElement* programmeInformationElement = doc->InsertEndChild(doc->NewElement("epg"))->ToElement();
		    programmeInformationElement->SetAttribute("xmlns", "http://www.worlddab.org/schemas/spi/31");
            programmeInformationElement->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
            programmeInformationElement->SetAttribute("xmlns:xml", "http://www.w3.org/XML/1998/namespace");
            programmeInformationElement->SetAttribute("xsi:schemaLocation", "http://www.worlddab.org/schemas/spi/31 spi_31.xsd");
            
            for(auto &schedule : programme_info.Schedules())
            {
                programmeInformationElement->InsertEndChild(build_schedule(doc, schedule));
            }         
             
            // stream to byte vector
            XMLPrinter printer;  
            doc->Accept(&printer);
            const char *s = printer.CStr();
            vector<unsigned char> bytes(s, s + strlen(s));
            return bytes;
        }
    
        vector<unsigned char> XmlMarshaller::Marshall(GroupInfo group_info) const
        {
            vector<unsigned char> bytes;
            return bytes;
        }    
        
        string XmlMarshaller::build_datetime(const DateTime &datetime) const
        {
            time_t now_c = chrono::system_clock::to_time_t(datetime);
            stringstream ss;
            ss << put_time(localtime(&now_c), "%Y-%m-%dT%H:%M:%S");
            return ss.str();
        }
        
        string XmlMarshaller::build_duration(const Duration &duration) const
        {
            stringstream ss;
            ss << "PT" << duration.count() << "S";
            return ss.str();
        }
        
        XMLElement* XmlMarshaller::build_schedule(XMLDocument* doc, Schedule &schedule) const
        {
            XMLElement* scheduleElement = doc->NewElement("schedule");
        
            // TODO print out timezone, rather than relying on local time
            auto sss = build_datetime(schedule.Created());
            const char* date_string = sss.c_str(); 
            scheduleElement->SetAttribute("creationTime", date_string);
            
            // originator
            if(!schedule.Originator().empty())
            {
                scheduleElement->SetAttribute("originator", schedule.Originator().c_str());
            }
            
            // version
            if(schedule.Version() > 0)
            {
                scheduleElement->SetAttribute("version", schedule.Version());
            } 
            
            // scope      
            XMLElement* scopeElement = doc->NewElement("scope");
            pair<DateTime, DateTime> scope = schedule.Scope();
            DateTime undefined_datetime = system_clock::from_time_t(0); // TODO better check for this
            if(scope.first != undefined_datetime && scope.second != undefined_datetime)  
            {
                {
                    auto sss = build_datetime(scope.first);
                    const char* start_date_string = sss.c_str(); 
                    scopeElement->SetAttribute("startTime", start_date_string);                       
                }
                {
                    auto sss = build_datetime(scope.second);
                    const char* stop_date_string = sss.c_str(); 
                    scopeElement->SetAttribute("stopTime", stop_date_string);                  
                }
                scheduleElement->InsertEndChild(scopeElement);
            }
            
            // programmes
            for(auto &programme : schedule.Programmes()) 
            {
                scheduleElement->InsertEndChild(build_programme(doc, programme));
            }
            
            return scheduleElement;
        }   
        
        XMLElement* XmlMarshaller::build_programme(XMLDocument* doc, Programme &programme) const
        {        
            XMLElement* programmeElement = doc->NewElement("programme");  
            programmeElement->SetAttribute("shortId", programme.ShortId());
            programmeElement->SetAttribute("id", programme.Id().c_str());              
                
            // names
            for(auto &name : programme.Names())
            {
                XMLElement* nameElement = build_name(doc, name);
                programmeElement->InsertEndChild(nameElement);
            }            
            
            // descriptions
            for(auto &description : programme.Descriptions())
            {
                XMLElement* descriptionElement = build_description(doc, description);
                programmeElement->InsertEndChild(descriptionElement);
            }     
            
            // locations
            for(auto &location : programme.Locations())
            {
                XMLElement* locationElement = build_location(doc, location);
                programmeElement->InsertEndChild(locationElement);
            }
            
            // media
            for(auto &media : programme.Media())
            {
                XMLElement* mediaElement = build_media(doc, media);
                programmeElement->InsertEndChild(mediaElement);
            }         
            
            // genre
            for(auto &genre : programme.Genres())
            {
                XMLElement* genreElement = build_genre(doc, genre);
                programmeElement->InsertEndChild(genreElement);
            }
            
            // keywords
            if(programme.Keywords().size() > 0)
            {
                vector<string> keywords = programme.Keywords();
                XMLElement* keywordsElement = build_keywords(doc, keywords);
                programmeElement->InsertEndChild(keywordsElement);
            }

            // links
            for(auto &link : programme.Links())
            {
                XMLElement* linkElement = build_link(doc, link);
                programmeElement->InsertEndChild(linkElement);
            }                
                
            // memberships
            for(auto &membership : programme.Memberships())
            {
                XMLElement* membershipElement = build_membership(doc, membership);
                programmeElement->InsertEndChild(membershipElement);
            }  

            // events
            for(auto &event : programme.Events())
            {
                XMLElement* eventElement = build_event(doc, event);
                programmeElement->InsertEndChild(eventElement);
            }
            
            return programmeElement;    
        }

        XMLElement* XmlMarshaller::build_event(XMLDocument* doc, ProgrammeEvent &event) const
        {        
            XMLElement* programmeEventElement = doc->NewElement("programmeEvent");  
            programmeEventElement->SetAttribute("shortId", event.ShortId());
            programmeEventElement->SetAttribute("id", event.Id().c_str());              
                
            // names
            for(auto &name : event.Names())
            {
                XMLElement* nameElement = build_name(doc, name);
                programmeEventElement->InsertEndChild(nameElement);
            }            
            
            // descriptions
            for(auto &description : event.Descriptions())
            {
                XMLElement* descriptionElement = build_description(doc, description);
                programmeEventElement->InsertEndChild(descriptionElement);
            }     
            
            // locations
            for(auto &location : event.Locations())
            {
                XMLElement* locationElement = build_location(doc, location);
                programmeEventElement->InsertEndChild(locationElement);
            }
            
            // media
            for(auto &media : event.Media())
            {
                XMLElement* mediaElement = build_media(doc, media);
                programmeEventElement->InsertEndChild(mediaElement);
            }         
            
            // genre
            for(auto &genre : event.Genres())
            {
                XMLElement* genreElement = build_genre(doc, genre);
                programmeEventElement->InsertEndChild(genreElement);
            }
            
            // keywords
            if(event.Keywords().size() > 0)
            {
                vector<string> keywords = event.Keywords();
                XMLElement* keywordsElement = build_keywords(doc, keywords);
                programmeEventElement->InsertEndChild(keywordsElement);
            }

            // links
            for(auto &link : event.Links())
            {
                XMLElement* linkElement = build_link(doc, link);
                programmeEventElement->InsertEndChild(linkElement);
            }                
                
            // memberships
            for(auto &membership : event.Memberships())
            {
                XMLElement* membershipElement = build_membership(doc, membership);
                programmeEventElement->InsertEndChild(membershipElement);
            }  
            
            return programmeEventElement;    
        }
        
        XMLElement* XmlMarshaller::build_location(XMLDocument* doc, Location &location) const
        {
            XMLElement* locationElement = doc->NewElement("location");
            
            // bearers
            for(auto &bearer : location.Bearers())
            {
                XMLElement* bearerElement = build_bearer(doc, bearer);
                locationElement->InsertEndChild(bearerElement);
            }
            
            // times
            for(auto &time : location.AbsoluteTimes())
            {
                XMLElement* timeElement = doc->NewElement("time");
                timeElement->SetAttribute("time", build_datetime(time.BilledTime()).c_str());
                timeElement->SetAttribute("duration", build_duration(time.BilledDuration()).c_str());
                timeElement->SetAttribute("actualTime", build_datetime(time.ActualTime()).c_str());
                timeElement->SetAttribute("actualDuration", build_duration(time.ActualDuration()).c_str());
                locationElement->InsertEndChild(timeElement);
            }
            for(auto &time : location.RelativeTimes())
            {
                XMLElement* timeElement = doc->NewElement("relativeTime");
                timeElement->SetAttribute("time", build_duration(time.BilledTime()).c_str());
                timeElement->SetAttribute("duration", build_duration(time.BilledDuration()).c_str());
                timeElement->SetAttribute("actualTime", build_duration(time.ActualTime()).c_str());
                timeElement->SetAttribute("actualDuration", build_duration(time.ActualDuration()).c_str());                
                locationElement->InsertEndChild(timeElement);
            }
            
            return locationElement;
        }
        
        XMLElement* XmlMarshaller::build_membership(XMLDocument* doc, Membership &membership) const
        {
            XMLElement* membershipElement = doc->NewElement("memberOf");
            membershipElement->SetAttribute("id", membership.Id().c_str());
            membershipElement->SetAttribute("shortId", membership.ShortId());
            return membershipElement;
        }        
        
        XMLElement* XmlMarshaller::build_bearer(XMLDocument* doc, Bearer *bearer) const
        {
            XMLElement* bearerElement = doc->NewElement("bearer");
            bearerElement->SetAttribute("id", bearer->URI().c_str());
            bearerElement->SetAttribute("cost", bearer->Cost());
            
            if(bearer->Cost() > 0)
            {
                bearerElement->SetAttribute("cost", bearer->Cost());
            }
            
            if(bearer->Offset() > 0)
            {
                bearerElement->SetAttribute("offset", bearer->Offset());
            }   
            
            if(dynamic_cast<DigitalBearer*>(bearer) != nullptr) 
            {
                DigitalBearer* digitalBearer = dynamic_cast<DigitalBearer*>(bearer);
                if(!digitalBearer->Content().empty())
                {
                    bearerElement->SetAttribute("mimeValue", digitalBearer->Content().c_str());
                }  
                if(digitalBearer->Bitrate() > 0)
                {
                    bearerElement->SetAttribute("bitrate", digitalBearer->Bitrate());
                }     
            }      
            
            return bearerElement;                                      
        }

        XMLElement* XmlMarshaller::build_service(XMLDocument* doc, Service &service) const
        {
            XMLElement* serviceElement = doc->NewElement("service");
            
            // bearers
            for(auto *bearer : service.Bearers())
            {
                XMLElement* bearerElement = build_bearer(doc, bearer);                                  
                serviceElement->InsertEndChild(bearerElement);
            }
            
            // names
            for(auto &name : service.Names())
            {
                XMLElement* nameElement = build_name(doc, name);
                serviceElement->InsertEndChild(nameElement);
            }            
            
            // descriptions
            for(auto &description : service.Descriptions())
            {
                XMLElement* descriptionElement = build_description(doc, description);
                serviceElement->InsertEndChild(descriptionElement);
            }     
            
            // media
            for(auto &media : service.Media())
            {
                XMLElement* mediaElement = build_media(doc, media);
                serviceElement->InsertEndChild(mediaElement);
            }         
            
            // genre
            for(auto &genre : service.Genres())
            {
                XMLElement* genreElement = build_genre(doc, genre);
                serviceElement->InsertEndChild(genreElement);
            }
            
            // keywords
            if(service.Keywords().size() > 0)
            {
                vector<string> keywords = service.Keywords();
                XMLElement* keywordsElement = build_keywords(doc, keywords);
                serviceElement->InsertEndChild(keywordsElement);
            }

            // links
            for(auto &link : service.Links())
            {
                XMLElement* linkElement = build_link(doc, link);
                serviceElement->InsertEndChild(linkElement);
            }
            
            // radiodns
            XMLElement* radiodnsElement = doc->NewElement("radiodns");
            radiodnsElement->SetAttribute("fqdn", service.FQDN().c_str());
            radiodnsElement->SetAttribute("serviceIdentifier", service.ServiceIdentifier().c_str());
            serviceElement->InsertEndChild(radiodnsElement);
            
            return serviceElement;            
        }     
        
        XMLElement* XmlMarshaller::build_name(XMLDocument* doc, Name &name) const
        {
            XMLElement* nameElement;
            if(name.MaxLength() <= 8)
            {
                nameElement = doc->NewElement("shortName");
            }
            else if(name.MaxLength() <= 16)
            {
                nameElement = doc->NewElement("mediumName");
            }
            else
            {
                nameElement = doc->NewElement("longName");
            }
            nameElement->SetText(name.Text().c_str());

            return nameElement;
        }
        
        XMLElement* XmlMarshaller::build_description(XMLDocument* doc, Description &description) const
        {
            XMLElement* mediaDescriptionElement = doc->NewElement("mediaDescription");
            XMLElement* descriptionElement;
            if(description.MaxLength() <= 180)
            {
                descriptionElement = doc->NewElement("shortDescription");
            }
            else
            {
                descriptionElement = doc->NewElement("longDescription");
            }
            descriptionElement->SetText(description.Text().c_str());
            mediaDescriptionElement->InsertEndChild(descriptionElement);

            return mediaDescriptionElement;
        }   
        
        XMLElement* XmlMarshaller::build_media(XMLDocument* doc, Multimedia &media) const
        {
            XMLElement* mediaDescriptionElement = doc->NewElement("mediaDescription");
            XMLElement* multimediaElement = doc->NewElement("multimedia");
            multimediaElement->SetAttribute("url", media.Location().c_str());
            
            if(!media.Content().empty())
            {
                multimediaElement->SetAttribute("mimeValue", media.Content().c_str());
            }  
            
            if(media.Width() > 0)
            {
                multimediaElement->SetAttribute("width", media.Width());
            }
            
            if(media.Height() > 0)
            {
                multimediaElement->SetAttribute("height", media.Height());
            }
            

            mediaDescriptionElement->InsertEndChild(multimediaElement);
            return mediaDescriptionElement;
        }
        
        XMLElement* XmlMarshaller::build_genre(XMLDocument* doc, Genre &genre) const
        {
            XMLElement* genreElement = doc->NewElement("genre");
            genreElement->SetAttribute("href", genre.Href().c_str());
            return genreElement;
        }
        
        XMLElement* XmlMarshaller::build_keywords(XMLDocument* doc, vector<string> &keywords) const
        {
            stringstream ss;
            copy(keywords.begin(), keywords.end(), ostream_iterator<string>(ss, ","));            
            XMLElement* keywordsElement = doc->NewElement("keywords");
            keywordsElement->SetText(ss.str().c_str());
            return keywordsElement;
        }   
        
        XMLElement* XmlMarshaller::build_link(XMLDocument* doc, Link &link) const
        {
            XMLElement* linkElement = doc->NewElement("link");
            linkElement->SetAttribute("uri", link.URI().c_str());
                       
            if(!link.Content().empty())
            {
                linkElement->SetAttribute("mimeValue", link.Content().c_str());
            }  
            
            if(!link.Description().empty())
            {
                linkElement->SetAttribute("description", link.Description().c_str());
            }  
            
            return linkElement;
        }
        
        /**
         * Unmarshalling
         */
        ServiceInfo XmlMarshaller::UnmarshallServiceInfo(vector<unsigned char> bytes) const
        {
            ServiceInfo info;

            XMLDocument doc;
            doc.Parse(reinterpret_cast<const char *>(&(bytes[0])));
            XMLElement* root = doc.FirstChildElement();
            XMLElement* servicesElement = root->FirstChildElement("services");

            // creation time
            const char* creationTimeAttribute = root->Attribute("creationTime");
            if(creationTimeAttribute != nullptr) {
                DateTime creationTime = parse_datetime(creationTimeAttribute);
            }

            // originator
            const char* originatorAttribute = root->Attribute("originator");
            if(originatorAttribute != nullptr) {
                info.SetOriginator(string(originatorAttribute));
            }            

            // TODO parse provider info

            XMLElement* serviceElement = servicesElement->FirstChildElement("service");
            while(serviceElement != nullptr)
            {
                Service service = parse_service(serviceElement);
                info.AddService(service);
                serviceElement = serviceElement->NextSiblingElement("service");
            }
            
            return info;
        }  
                    
        ProgrammeInfo XmlMarshaller::UnmarshallProgrammeInfo(vector<unsigned char> bytes) const
        {
            XMLDocument doc;
            doc.Parse(reinterpret_cast<const char *>(&(bytes[0])));
            XMLElement* root = doc.FirstChildElement();

            ProgrammeInfo info;
            XMLElement* scheduleElement = root->FirstChildElement("schedule");
            while(scheduleElement != nullptr)
            {
                Schedule schedule = parse_schedule(scheduleElement);
                info.AddSchedule(schedule);
                scheduleElement = root->NextSiblingElement("schedule");
            }

            return info;
        }
            
        GroupInfo XmlMarshaller::UnmarshallGroupInfo(vector<unsigned char> bytes) const
        {
            GroupInfo info;
            return info;
        }           

        
        Service XmlMarshaller::parse_service(XMLElement* serviceElement) const
        {                        
            // identifier
            XMLElement* radiodnsElement = serviceElement->FirstChildElement("radiodns");
            string fqdn(radiodnsElement->Attribute("fqdn"));
            string serviceIdentifier(radiodnsElement->Attribute("serviceIdentifier"));
            Service service(fqdn, serviceIdentifier);
            
            // names
            XMLElement* shortnameElement = serviceElement->FirstChildElement("shortName");
            while(shortnameElement != nullptr)
            {
                service.AddName(parse_name(shortnameElement));
                shortnameElement = shortnameElement->NextSiblingElement("shortName");
            }
            XMLElement* mediumnameElement = serviceElement->FirstChildElement("mediumName");
            while(mediumnameElement != nullptr)
            {
                service.AddName(parse_name(mediumnameElement));
                mediumnameElement = mediumnameElement->NextSiblingElement("mediumName");
            }
            XMLElement* longnameElement = serviceElement->FirstChildElement("longName");
            while(longnameElement != nullptr)
            {
                service.AddName(parse_name(longnameElement));
                longnameElement = longnameElement->NextSiblingElement("longName");
            }

            // descriptions
            XMLElement* mediaDescriptionElement = serviceElement->FirstChildElement("mediaDescription");
            while(mediaDescriptionElement != nullptr)
            {
                XMLElement* shortDescriptionElement = mediaDescriptionElement->FirstChildElement("shortDescription");
                if(shortDescriptionElement != nullptr)
                {
                    service.AddDescription(parse_description(shortDescriptionElement));
                }
                XMLElement* longDescriptionElement = mediaDescriptionElement->FirstChildElement("longDescription");
                if(longDescriptionElement != nullptr)            
                {
                    service.AddDescription(parse_description(longDescriptionElement));                }
                mediaDescriptionElement = mediaDescriptionElement->NextSiblingElement("mediaDescription");
            }
   
            // logos
            mediaDescriptionElement = serviceElement->FirstChildElement("mediaDescription");
            while(mediaDescriptionElement != nullptr)
            {
                XMLElement* multimediaElement = mediaDescriptionElement->FirstChildElement("multimedia");
                if(multimediaElement != nullptr)
                {
                    string url(multimediaElement->Attribute("url"));

                    // mime type is required unless the type is defined as logo_colour_square or logo_colour_rectangle
                    const char* type = multimediaElement->Attribute("type");
                    const char* mimeValue = multimediaElement->Attribute("mimeValue");
                    string content;
                    if(type != nullptr && (strcmp(type, "logo_colour_square") == 0 || strcmp(type, "logo_colour_rectangle") == 0))
                    {
                        content = string("image/png");
                    }
                    else if(mimeValue != nullptr)
                    {
                        content = string(mimeValue);
                    }
                    else
                    {
                        // TODO throw parsing error
                        cout << "Error - doesnt look like there is either a mime type or a recognised type against this logo: " << url << endl;
                    }

                    Multimedia media(url, content);

                    const char* heightAttribute = multimediaElement->Attribute("height");
                    const char* widthAttribute = multimediaElement->Attribute("width");
                    if(heightAttribute != nullptr && widthAttribute != nullptr)
                    {
                        media.SetHeight(multimediaElement->IntAttribute("height"));
                        media.SetWidth(multimediaElement->IntAttribute("width"));
                    }

                    service.AddMedia(media);
                }
                mediaDescriptionElement = mediaDescriptionElement->NextSiblingElement("mediaDescription");                    
            }
            
            // genres
            XMLElement* genreElement = serviceElement->FirstChildElement("genre");
            while(genreElement != nullptr)
            {
                string href(genreElement->Attribute("href"));
                Genre genre(href);
                service.AddGenre(genre);
                genreElement = genreElement->NextSiblingElement("genre");
            }

            // links
            XMLElement* linkElement = serviceElement->FirstChildElement("link");
            while(linkElement != nullptr)
            {
                string uri(linkElement->Attribute("uri"));
                Link link(uri);
                const char* mimeValue = linkElement->Attribute("mimeValue");
                if(mimeValue != nullptr)
                {
                    link.SetContent(string(mimeValue));
                }
                service.AddLink(link);
                linkElement = linkElement->NextSiblingElement("link");
            }

            // bearers
            XMLElement* bearerElement = serviceElement->FirstChildElement("bearer");
            while(bearerElement != nullptr)
            {
                string uri(bearerElement->Attribute("id"));
                Bearer* bearer;
                
                // parse the specific bearer
                if(uri.find("dab:") != string::npos)
                {
                    string content(bearerElement->Attribute("mimeValue"));
                    bearer = new DabBearer(uri, content);
                }
                else if(uri.find("fm:") != string::npos)
                {
                    bearer = new FmBearer(uri);
                }
                else if(uri.find("http:") != string::npos)
                {
                    string content(bearerElement->Attribute("mimeValue"));
                    bearer = new IpBearer(uri, content);
                }
                
                // and the common attributes
                if(bearerElement->Attribute("cost") != nullptr)
                {
                    int n;
                    istringstream(bearerElement->Attribute("cost")) >> n;
                    bearer->SetCost(n);
                }
                if(bearerElement->Attribute("offset") != nullptr)
                {
                    int n;
                    istringstream(bearerElement->Attribute("offset")) >> n;
                    bearer->SetOffset(n);                    
                }

                service.AddBearer(bearer);
                bearerElement = bearerElement->NextSiblingElement("bearer");
            }

            return service;    
        }
        
        Name XmlMarshaller::parse_name(XMLElement* element) const
        {
            string text = element->GetText();
            if(strcmp(element->Name(), "shortName") == 0)
            {
                return ShortName(text);
            }
            if(strcmp(element->Name(), "mediumName") == 0)
            {
                return MediumName(text); 
            }
            else
            {
                return LongName(text); 
            }
        }
        
        Description XmlMarshaller::parse_description(XMLElement* element) const
        {
            string text = element->GetText();
            if(strcmp(element->Name(), "shortDescription") == 0)
            {
                return ShortDescription(text);
            }
            else
            {
                return LongDescription(text);
            }
        }

        Schedule XmlMarshaller::parse_schedule(XMLElement* element) const
        {
            Schedule schedule;
            
            // programmes
            XMLElement* programmeElement = element->FirstChildElement("programme");
            while(programmeElement != nullptr)
            {
                Programme programme = parse_programme(programmeElement);
                schedule.AddProgramme(programme);
                programmeElement = programmeElement->NextSiblingElement("programme");
            }                

            return schedule;
        }

        Programme XmlMarshaller::parse_programme(XMLElement* programmeElement) const
        {
            // identifier
            string id = programmeElement->Attribute("id");
            int shortId = atoi(programmeElement->Attribute("shortId"));
            Programme programme(id, shortId);

            // names
            XMLElement* shortnameElement = programmeElement->FirstChildElement("shortName");
            while(shortnameElement != nullptr)
            {
                programme.AddName(parse_name(shortnameElement));
                shortnameElement = programmeElement->NextSiblingElement("shortName");
            }
            XMLElement* mediumnameElement = programmeElement->FirstChildElement("mediumName");
            while(mediumnameElement != nullptr)
            {
                programme.AddName(parse_name(mediumnameElement));
                mediumnameElement = programmeElement->NextSiblingElement("mediumName");
            }
            XMLElement* longnameElement = programmeElement->FirstChildElement("longName");
            while(longnameElement != nullptr)
            {
                programme.AddName(parse_name(longnameElement));
                longnameElement = longnameElement->NextSiblingElement("longName");
            }

            // descriptions
            XMLElement* mediaDescriptionElement = programmeElement->FirstChildElement("mediaDescription");
            while(mediaDescriptionElement != nullptr)
            {
                XMLElement* shortDescriptionElement = mediaDescriptionElement->FirstChildElement("shortDescription");
                if(shortDescriptionElement != nullptr)
                {
                    programme.AddDescription(parse_description(shortDescriptionElement));
                }
                XMLElement* longDescriptionElement = mediaDescriptionElement->FirstChildElement("longDescription");
                if(longDescriptionElement != nullptr)            
                {
                    programme.AddDescription(parse_description(longDescriptionElement));                
                }
                mediaDescriptionElement = mediaDescriptionElement->NextSiblingElement("mediaDescription");
            }            

            // logos
            mediaDescriptionElement = programmeElement->FirstChildElement("mediaDescription");
            while(mediaDescriptionElement != nullptr)
            {
                XMLElement* multimediaElement = mediaDescriptionElement->FirstChildElement("multimedia");
                if(multimediaElement != nullptr)
                {
                    string url(multimediaElement->Attribute("url"));

                    // mime type is required unless the type is defined as logo_colour_square or logo_colour_rectangle
                    const char* type = multimediaElement->Attribute("type");
                    const char* mimeValue = multimediaElement->Attribute("mimeValue");
                    string content;
                    if(type != nullptr && (strcmp(type, "logo_colour_square") == 0 || strcmp(type, "logo_colour_rectangle") == 0))
                    {
                        content = string("image/png");
                    }
                    else if(mimeValue != nullptr)
                    {
                        content = string(mimeValue);
                    }
                    else
                    {
                        // TODO throw parsing error
                        cout << "Error - doesnt look like there is either a mime type or a recognised type against this logo: " << url << endl;
                    }

                    Multimedia media(url, content);

                    const char* heightAttribute = multimediaElement->Attribute("height");
                    const char* widthAttribute = multimediaElement->Attribute("width");
                    if(heightAttribute != nullptr && widthAttribute != nullptr)
                    {
                        media.SetHeight(multimediaElement->IntAttribute("height"));
                        media.SetWidth(multimediaElement->IntAttribute("width"));
                    }

                    programme.AddMedia(media);
                }
                mediaDescriptionElement = mediaDescriptionElement->NextSiblingElement("mediaDescription");                    
            }
            
            // genres
            XMLElement* genreElement = programmeElement->FirstChildElement("genre");
            while(genreElement != nullptr)
            {
                string href(genreElement->Attribute("href"));
                Genre genre(href);
                programme.AddGenre(genre);
                genreElement = genreElement->NextSiblingElement("genre");
            }

            // links
            XMLElement* linkElement = programmeElement->FirstChildElement("link");
            while(linkElement != nullptr)
            {
                string uri(linkElement->Attribute("uri"));
                Link link(uri);
                const char* mimeValue = linkElement->Attribute("mimeValue");
                if(mimeValue != nullptr)
                {
                    link.SetContent(string(mimeValue));
                }
                programme.AddLink(link);
                linkElement = linkElement->NextSiblingElement("link");
            }

            // locations
            XMLElement* locationElement = programmeElement->FirstChildElement("location");
            while(locationElement != nullptr)     
            {
                Location location = parse_location(locationElement);
                programme.AddLocation(location);
                locationElement = programmeElement->NextSiblingElement("location");
            }       

            return programme;
        }

        Location XmlMarshaller::parse_location(XMLElement* locationElement) const
        {
            Location location;

            // times
            XMLElement* timeElement = locationElement->FirstChildElement("time");
            while(timeElement != nullptr)     
            {
                AbsoluteTime time(parse_datetime(timeElement->Attribute("time")), 
                                  parse_duration(timeElement->Attribute("duration")));
                location.AddAbsoluteTime(time);
                timeElement = timeElement->NextSiblingElement("location");

            }   

            return location;
        }

        DateTime parse_datetime(string input)
        {
            struct tm tm;
            std::istringstream iss(input);
            iss >> std::get_time(&tm, "%FT%T%z");
            return std::chrono::system_clock::from_time_t(mktime(&tm));
        }

        Duration parse_duration(string input)
        {
            std::regex r("^P([0-9]+Y|)?([0-9]+M|)?([0-9]+D|)?T?([0-9]+H|)?([0-9]+M|)?([0-9]+S|)?$");
            std::smatch match;
            if(std::regex_search(input, match, r) && match.size() > 1)
            {
                try
                {
                    int seconds = 0;
                    if(match.length(6)) seconds += std::stoi(match.str(6));
                    if(match.length(5)) seconds += (std::stoi(match.str(5)) * 60);
                    if(match.length(4)) seconds += (std::stoi(match.str(4)) * 60 * 60);
                    if(match.length(3)) seconds += (std::stoi(match.str(3)) * 24 * 60 * 60);
                    return Duration(seconds);
                } 
                catch(exception& e) 
                {
                    throw invalid_duration(input);
                }
            } else {
                throw invalid_duration(input);
            }
        }


    }        
            
}
