#ifndef HYBRIDSPI_SERVICES_H
#define HYBRIDSPI_SERVICES_H

#include <hybridspi/common.h>
#include <iomanip>
#include <iterator>

using namespace std;
using namespace hybridspi;


namespace hybridspi
{
    
    class Service : public Named, public Described, public GeoLocated, public Linked, public Genred, public MultimediaEnabled, public Keyworded
    {
        
        public:
        
            Service(string lookup);
            
            void AddBearer(Bearer bearer);
            
            void RemoveBearer(Bearer bearer);
            
            vector<Bearer*> Bearers() const { return bearers; };
            
            bool operator== (const Service &that) const;
            
            bool operator!= (const Service &that) const;                  
                        
            
        private:
        
            string lookup;
            
            vector<Bearer*> bearers;
        
    };    
    
    class ServiceProvider: public Named, public Described, public MultimediaEnabled, public Linked, public Keywords
    {
        
        public:
        
            ServiceProvider();
        
    };
    
    class ServiceInfo
    {
        
        public:
        
            ServiceInfo();
        
            ServiceInfo(DateTime created, int version);
        
            //ServiceInfo(DateTime created, int version, string originator, ServiceProvider provider);
            
            DateTime GetCreated() const { return created; };
            
            int GetVersion() const { return version; };
            
            string GetOriginator() const { return originator; };
            
            //ServiceProvider GetServiceProvider() { return provider; };
            
            void AddService(Service service);
            
            vector<Service> Services() const { return services; };
            
            void RemoveService(const Service &service);
            
            friend ostream& operator<< (ostream& os, const ServiceInfo& info);         
        
        private:
        
            DateTime created;
            
            int version;
            
            string originator;
            
            //ServiceProvider provider;
            
            vector<Service> services;
        
    };
    
}

#endif