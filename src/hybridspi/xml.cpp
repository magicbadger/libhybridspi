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
            vector<unsigned char> bytes;
            return bytes;
        }
    
        vector<unsigned char> XmlMarshaller::Marshall(GroupInfo group_info) const
        {
            vector<unsigned char> bytes;
            return bytes;
        }       

        XMLElement* XmlMarshaller::build_service(XMLDocument* doc, Service &service) const
        {
            XMLElement* serviceElement = doc->NewElement("service");
            
            // bearers
            for(auto *bearer : service.Bearers())
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
            if(description.MaxLength() <= 128)
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
            XMLDocument doc;
            doc.Parse(reinterpret_cast<const char *>(&(bytes[0])));
            XMLElement* root = doc.FirstChildElement();
            XMLElement* servicesElement = root->FirstChildElement("services");

            ServiceInfo info;
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
            ProgrammeInfo info;
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
    }        
            
}