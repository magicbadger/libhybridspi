#include <hybridspi/xml.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace tinyxml2;

namespace hybridspi
{
    namespace xml
    {
        XmlMarshaller::XmlMarshaller()
            : Marshaller()
        { }

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

        XMLElement* XmlMarshaller::build_service(XMLDocument* doc, Service &service) const
        {
            XMLElement* serviceElement = doc->NewElement("service");
            
            // bearers
            for(auto *bearer : service.Bearers())
            {
                XMLElement* bearerElement = doc->NewElement("bearer");
                bearerElement->SetAttribute("uri", bearer->URI().c_str());
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
     
                        
    }
}