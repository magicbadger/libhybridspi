#include <hybridspi/binary.h>

using namespace std;
using namespace std::chrono;

namespace hybridspi
{
    namespace binary
    {
        
        template<size_t size>
        inline std::vector<unsigned char> bits_to_bytes(std::bitset<size> bits)
        {
            std::vector<unsigned char> bytes(size/8);
            for(int j=0; j<int(size); j++) {
                bytes[j/8] = bytes[j/8] + ((bits[size-j-1] ? 1:0) << (7-j%8));
            }

            return bytes;
        }
        
        inline std::vector<unsigned char> operator+(const std::vector<unsigned char> &a, const std::vector<unsigned char> &b)
        {
            std::vector<unsigned char> result(b);
            result.insert(result.begin(), a.begin(), a.end());
            return result;
        }
        
        Ensemble::Ensemble(int ecc, int eid)
            : ecc(ecc), eid(eid), Named(), Described(), MultimediaEnabled()
        { }
        
        BinaryMarshaller::BinaryMarshaller(Ensemble ensemble)
            : ensemble(ensemble)
        { }
        
        vector<unsigned char> BinaryMarshaller::Marshall(ServiceInfo info) const
        {
            // serviceInformation
            Element serviceInformationElement(0x03);
            serviceInformationElement.AddAttribute(Attribute(0x81, encode_timepoint(info.GetCreated())));
            
            vector<unsigned char> t = encode_number<16>(8);
            
            if(info.GetVersion() > 0)
            {
                Attribute versionAttribute(0x80, encode_number<16>(info.GetVersion()));
                serviceInformationElement.AddAttribute(versionAttribute);
            }
            if(!info.GetOriginator().empty())
            {
                Attribute originatorAttribute(0x80, encode_number<16>(info.GetVersion()));
                serviceInformationElement.AddAttribute(Attribute(0x82, info.GetOriginator()));
            }
            
            // ensemble
            Element ensembleElement = build_ensemble(ensemble, info);
            serviceInformationElement.AddElement(ensembleElement);
            
            return serviceInformationElement.encode();
        }
        
        vector<unsigned char> encode_string(string val)
        {
            vector<unsigned char> bytes;
            copy(val.begin(), val.end(), back_inserter(bytes));
            return bytes;
        }
        
        template<size_t size>
        vector<unsigned char> encode_number(int val)
        {
            bitset<size> data(val);
            return bits_to_bytes(data);
        }
            
        int timepoint_to_mjd(DateTime timepoint)
        {            
            time_t tt = system_clock::to_time_t(timepoint);
            tm tm = *gmtime(&tt);
            int day = tm.tm_mday;
            int month = tm.tm_mon + 1;
            int year = tm.tm_year + 1900;

            return
                367 * year
                - 7 * (year + (month + 9) / 12) / 4
                - 3 * ((year + (month - 9) / 7) / 100 + 1) / 4
                + 275 * month / 9
                + day
                + 1721028
                - 2400;  
        }        
                       
        vector<unsigned char> encode_timepoint(DateTime timepoint)
        {
            int mjd = timepoint_to_mjd(timepoint);
            system_clock::duration d = timepoint.time_since_epoch();
            seconds s = duration_cast<seconds>(d);
            
            if(s.count() > 0)
            {
                hours hh = duration_cast<hours>(s) % 24;
                minutes mm = duration_cast<minutes>(s % hours(1));
                seconds ss = duration_cast<seconds>(s % minutes(1));
                milliseconds msec = duration_cast<milliseconds>(s % seconds(1));

                // only seconds precision right now
                bitset<16> lower(0 + // millis (10)
                                (ss.count() << 10)); // seconds (6)
                bitset<32> bits((mm.count()) + // minutes (6)
                            (hh.count() << 6) + // hours (5)
                            (1 << 11) + // UTC flag (1)
                            (0 << 12) + // RFU (2)
                            (mjd << 14) + // MJD (17)
                            (1 << 31)); // validity (1)
                return bits_to_bytes(bits) + bits_to_bytes(lower);
            }
            else
            {
                hours hh = duration_cast<hours>(s) % 24;
                minutes mm = duration_cast<minutes>(s % hours(1));
                bitset<32> bits((mm.count()) + // minutes (6)
                            (hh.count() << 6) + // hours (5)
                            (0 << 11) + // UTC flag (1)
                            (0 << 12) + // RFU (2)
                            (mjd << 14) + // MDJ (17)
                            (1 << 31)); // validity (1)
                return bits_to_bytes(bits);
            }            
        }
        
        vector<unsigned char> encode_ensembleId(int ecc, int eid)
        {
            bitset<24> bits(eid + // EId (16) 
                            (ecc << 16)); // ECC (8)
            return bits_to_bytes(bits);   
        }
        
        Element::Element(int tag)
            : tag(tag)
        { }
        
        Element::Element(int tag, vector<Attribute> attributes)
            : tag(tag), attributes(attributes)
        { }
          
        Element::Element(int tag, vector<Attribute> attributes, vector<Element> elements)
            : tag(tag), attributes(attributes), elements(elements)
        { }
        
        Element::Element(int tag, vector<Attribute> attributes, vector<Element> elements, vector<unsigned char> cdata)
            : tag(tag), attributes(attributes), elements(elements), cdata(cdata)
        { }
        
        void Element::AddAttribute(Attribute attribute)
        {
            attributes.push_back(attribute);
        }
        
        void Element::AddElement(Element element)
        {
            elements.push_back(element);
        }
        
        void Element::SetCDATA(vector<unsigned char> cdata)
        {
            cdata = cdata;
        }
                
        vector<unsigned char> Element::encode() const
        {
            vector<unsigned char> value;
            
            // encode attributes   
            for(auto &attribute : attributes)
            {
                value = value + attribute.encode();
            }
            
            // encode children
            for(auto &element : elements)
            {
                value = value + element.encode();
            }
            
            // encode CDATA
            if(cdata.size() > 0)
            {
                value = value + cdata;
            }
            
            vector<unsigned char> data;
            
            // b0-b7: tag
            data = data + encode_number<8>(tag);

            // b8-15: element data length (0-253 bytes)
            if(value.size() <= 253)
            {
                data = data + encode_number<8>(value.size());             
            }
            // b8-31: extended element length (256-65536 bytes)            
            else if(value.size() >= 254 && value.size() <= 1<<16)
            {
                data = data + encode_number<24>(0xfe + (value.size() << 8));                                            
            }
            // b8-39: extended element length (65537-16777216 bytes)            
            else if(value.size() > 1<<16 && value.size() <= 1<<24)
            {
                data = data + encode_number<32>(0xff + (value.size() << 8));             
            }
            else
            {
                throw std::length_error("attribute value datalength exceeds the maximum allowed by the extended element length (24bits)");
            }
            
            data = data + value;
            
            return data;            
        }
        
        template<size_t size>
        Attribute::Attribute(int tag, int value)
            : tag(tag), value(encode_number<size>(value))
        { }
                
        Attribute::Attribute(int tag, string value)
            : tag(tag), value(encode_string(value))
        { }       
        
        Attribute::Attribute(int tag, vector<unsigned char> value)
            : tag(tag), value(value)
        { }

        vector<unsigned char> Attribute::encode() const
        {   
            vector<unsigned char> bytes;
            
            // b0-b7: tag
            bytes = bytes + encode_number<8>(tag);

            // b8-15: element data length (0-253 bytes)
            // b16-31: extended element length (256-65536 bytes)
            // b16-39: extended element length (65537-16777216 bytes)
            if(value.size() <= 253)
            {
                bytes = bytes + encode_number<8>(value.size());
            }
            else if(value.size() >= 254 && value.size() <= 1<<16)
            {
                bytes = bytes + encode_number<24>(0xfe + (value.size() << 8));
            }
            else if(value.size() > 1<<16 && value.size() <= 1<<24)
            {
                bytes = bytes + encode_number<32>(0xff + (value.size() < 8));             
            }
            else
            {
                throw std::length_error("attribute value datalength exceeds the maximum allowed by the extended element length (24bits)");
            }
            
            return bytes;
        }
        
        Element build_ensemble(Ensemble ensemble, ServiceInfo info)
        {
            Element ensembleElement(0x26);
            
            // ensemble ID 
            Attribute ensembleIdAttribute(0x80, encode_ensembleId(ensemble.ECC(), ensemble.EId()));
            ensembleElement.AddAttribute(ensembleIdAttribute);
            
            // names           
            for(auto &name : ensemble.GetNames())
            {
                Element nameElement = build_name(name);
                ensembleElement.AddElement(nameElement);
            }
            
            // descriptions
            for(auto &description : ensemble.GetDescriptions())
            {
                Element descriptionElement = build_description(description);
                ensembleElement.AddElement(descriptionElement);
            }
           
            // media
            for(auto &media : ensemble.GetMultimedia())
            {
                Element mediaElement = build_media(media);
                ensembleElement.AddElement(mediaElement);
            }
           
            // services
            for(auto &service : info.Services())
            {
                Element serviceElement = build_service(service);
                ensembleElement.AddElement(serviceElement);
            }
            
            return ensembleElement;
        }
        
        Element build_description(Description description)
        {
            int tag;
            if(name.MaxLength() <= 128) { tag = 0x1a; }
            else { tag = 0x1b; }
            
            Element mediagroupElement(0x13);
            Element descriptionElement(tag);
            descriptionElement.SetCDATA(encode_string(description.Text()));
            mediagroupElement.AddElement(descriptionElement);
            return mediagroupElement;
        }        
        
        Element build_name(Name name)
        {
            int tag;
            if(name.MaxLength() <= 8) { tag = 0x10; }
            else if(name.MaxLength() <= 16) { tag = 0x11; }
            else { tag = 0x12; }
            
            Element nameElement(tag);
            nameElement.SetCDATA(encode_string(name.Text()));
            return nameElement;
        }
        
        Element build_media(Multimedia media)
        {
    
            Element mediagroupElement(0x13);
            Element mediaElement(0x2b);
            
            // content type
            Attribute contentTypeAttribute(0x80, encode_string(media.ContentType()));
            mediaElement.AddAttribute(contentTypeAttribute);
            
            // location
            Attribute locationAttribute(0x82, encode_string(media.Location()));
            mediaElement.AddAttribute(locationAttribute);
            
            // dimensions
            if(media.Height() > 0)
            {
                Attribute heightAttribute(0x84, encode_number<16>(media.Height()));
                mediaElement.AddAttribute(heightAttribute);
            }
            if(media.Width() > 0)
            {
                Attribute widthAttribute(0x84, encode_number<16>(media.Width()));
                mediaElement.AddAttribute(widthAttribute);
            }
            
            mediagroupElement.AddElement(mediaElement);
            return mediagroupElement;        
        }
        
        Element build_service(Service service)
        {
            Element serviceElement(0x28);
            
            // service ID
            for(auto &bearer : service.Bearers())
            {
                
            }
            
            
            
    service_element = Element(0x28)

    # version
    if service.version > 1: service_element.attributes.append(Attribute(0x80, service.version, encode_number, 16))

    # service IDs - the first in the list is primary, all others secondary
    for bearer in service.bearers:
        serviceid_element = Element(0x29)
        serviceid_element.attributes.append(Attribute(0x80, bearer, encode_bearer))
        service_element.children.append(serviceid_element)

    # names
    for name in service.names:
        service_element.children.append(build_name(name))

    # descriptions
    for description in service.descriptions:
        service_element.children.append(build_description(description))

    # media
    for media in service.media:
        service_element.children.append(build_mediagroup(media))

    # genre
    for genre in service.genres:
        service_element.children.append(build_genre(genre))

    # language TODO

    # keywords
    if len(service.keywords):
        service_element.children.append(build_keywords(service.keywords))


        }        
                

    }
}