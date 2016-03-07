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
        
        class Ensemble : public Named, public Described, public MediaEnabled
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
        
        class CData
        {
            public:
            
                CData();
            
                CData(vector<unsigned char> value);
                
                CData(string value);
                
                vector<unsigned char> encode() const;
                
                unsigned int size() const { return value.size(); };
                
                vector<unsigned char> Value() const { return value; };
                
            private:
            
                vector<unsigned char> value;
        };        
        
        class Element
        {
            
            public:
            
                Element(int tag);
                
                Element(int tag, vector<Attribute> attributes);
                
                Element(int tag, vector<Attribute> attributes, vector<Element> elements);
            
                Element(int tag, vector<Attribute> attributes, vector<Element> elements, CData data);
                
                void AddAttribute(Attribute attribute);
                
                vector<Attribute> Attributes() const { return attributes; };
                
                void AddElement(Element element);
                
                vector<Element> Elements() const { return elements; };
                
                void SetData(const CData& data);
                
                CData Data() const { return cdata; };
                
                vector<unsigned char> encode() const;
                
            private:
            
                int tag;
                
                vector<Attribute> attributes;
                
                vector<Element> elements;
                
                CData cdata;
            
        };
       
        
        // encode functions
        
        vector<unsigned char> encode_string(string val);
        
        template<size_t size>
        vector<unsigned char> encode_number(int number);
        
        unsigned int timepoint_to_mjd(DateTime timepoint);        
        
        vector<unsigned char> encode_timepoint(DateTime timepoint);
        
        vector<unsigned char> encode_ensembleId(int ecc, int eid);
        
        
        
        // build functions
        
        Element build_ensemble(Ensemble ensemble, ServiceInfo info);
        
        Element build_description(Description description);
        
        Element build_name(Name name);
        
        Element build_media(Multimedia media);
        
        Element build_service(Service service);
        
        Element build_genre(Genre& genre);
        
        Element build_bearer(Bearer* bearer);
        
        
        // exceptions
        
        class unable_to_encode_bearer: public exception
        {
            virtual const char* what() const throw()
            {
                return "Invalid bearer";
            }
        };
        
    }
}

#endif