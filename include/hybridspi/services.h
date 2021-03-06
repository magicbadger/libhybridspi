#ifndef HYBRIDSPI_SERVICES_H
#define HYBRIDSPI_SERVICES_H

#include <hybridspi/common.h>
#include <iomanip>
#include <iterator>

using namespace std;
using namespace hybridspi;


namespace hybridspi
{
    
    class Service : public Named, public Described, public GeoLocated, 
                    public Linked, public Genred, public MediaEnabled, public Keyworded
    {
        
        public:
        
            Service(string fqdn, string serviceIdentifier);
            
            string FQDN() { return fqdn; };
            
            string ServiceIdentifier() { return serviceIdentifier; };
            
            void AddBearer(Bearer *bearer);
            
            void RemoveBearer(const Bearer *bearer);
            
            vector<Bearer*> Bearers() const { return bearers; };
            
            bool operator== (const Service &that) const;
            
            bool operator!= (const Service &that) const;                  
                        
            
        private:
        
            string fqdn;
            
            string serviceIdentifier;
            
            vector<Bearer*> bearers;
        
    };    
    
    class ServiceProvider: public Named, public Described, public MediaEnabled, public Linked, 
                           public Keyworded
    {
        
        public:
        
            ServiceProvider();
        
    };
    
    class ServiceInfo
    {
        
        public:

            ServiceInfo(DateTime created = system_clock::now(), int version = 0);
            
            ServiceInfo* SetCreated(DateTime created)
            {
                this->created = created;
                return this;
            }
            
            DateTime Created() const { return created; };
            
            ServiceInfo* SetVersion(int version)
            {
                this->version = version;
                return this;
            }            
            
            int Version() const { return version; };
            
            ServiceInfo* SetOriginator(string originator)
            {
                this->originator = originator;
                return this;
            }
            
            string Originator() const { return originator; };
            
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