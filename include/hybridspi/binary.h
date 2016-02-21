#ifndef HYBRIDSPI_BINARY_H
#define HYBRIDSPI_BINARY_H

#include <hybridspi/common.h>
#include <hybridspi/services.h>
#include <hybridspi/marshall.h>

using namespace hybridspi;

namespace hybridspi
{
    namespace binary
    {
        
        class Ensemble : public Named, public Described, public MultimediaEnabled
        {
            public:
            
                Ensemble(int ecc, int eid);
                
                int ECC() const
                {
                    return ecc;
                }
                
                int EId() const
                {
                    return eid;
                }
                
            private:
                
                int ecc;
                
                int eid;
            
        };
        
        class BinaryMarshaller: public Marshaller     
        {
            public:
            
                BinaryMarshaller(Ensemble ensemble);
            
                vector<unsigned char> Marshall(ServiceInfo service_info) const;
                
            private:
            
                Ensemble ensemble;
        };
        
        class Attribute
        {
            public:
                
                template<size_t size>
                Attribute(int tag, int value);
                
                Attribute(int tag, string value);
                
                Attribute(int tag, vector<unsigned char> value);
                
                vector<unsigned char> encode() const;
                
            private:
            
                int tag;
                
                vector<unsigned char> value;
            
        };
        
        class Element
        {
            
            public:
            
                Element(int tag);
                
                Element(int tag, vector<Attribute> attributes);
                
                Element(int tag, vector<Attribute> attributes, vector<Element> elements);
            
                Element(int tag, vector<Attribute> attributes, vector<Element> elements, vector<unsigned char> cdata);
                
                void AddAttribute(Attribute attribute);
                
                void AddElement(Element element);
                
                void SetCDATA(vector<unsigned char> cdata);
                
                vector<unsigned char> encode() const;
                
            private:
            
                int tag;
                
                vector<Attribute> attributes;
                
                vector<Element> elements;
                
                vector<unsigned char> cdata;
            
        };
        
        
        
        // encode functions
        
        vector<unsigned char> encode_string(string val);
        
        template<size_t size>
        vector<unsigned char> encode_number(int number);
        
        int timepoint_to_mjd(int timepoint);        
        
        vector<unsigned char> encode_timepoint(DateTime timepoint);
        
        vector<unsigned char> encode_ensembleId(int ecc, int eid);
        
        
        
        // build functions
        
        Element build_ensemble(Ensemble ensemble, ServiceInfo info);
        
        Element build_description(Description description);
        
        Element build_name(Name name);
        
        Element build_media(Multimedia media);
        
        Element build_service(Service service);
        
    }
}

#endif