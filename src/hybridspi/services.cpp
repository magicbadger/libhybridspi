#include <hybridspi/services.h>

namespace hybridspi
{
        
    ServiceProvider::ServiceProvider()
        : Named(), Described(), MediaEnabled(), Linked(), Keyworded()
    { }
    
    ServiceInfo::ServiceInfo(DateTime created, int version)
        : created(created), version(version)
    { }
    
    void ServiceInfo::AddService(Service service)
    {
        services.push_back(service);
    }
    
    void ServiceInfo::RemoveService(const Service &service)
    {
        auto it = std::find(services.begin(), services.end(), service);
        if(it != services.end())
            services.erase(it);
    }    
    
    ostream& operator<<(ostream& os, const ServiceInfo& info)
    {
        time_t time = system_clock::to_time_t(info.Created());
        string txt("ServiceInfo: services=");
        os << txt;
        return os;
    }
    
    Service::Service(string lookup)
        : lookup(lookup), Named(), Described(), GeoLocated(), Linked(), Genred(), MediaEnabled(), Keyworded() 
    { }
    
    void Service::AddBearer(Bearer* bearer)
    {
        bearers.push_back(bearer);
    }
    
    void Service::RemoveBearer(const Bearer *bearer)
    {
        auto it = std::find(bearers.begin(), bearers.end(), bearer);
        if(it != bearers.end())
            bearers.erase(it);
    }
    
    bool Service::operator== (const Service &that) const
    {
        return lookup == that.lookup;
    }
            
    bool Service::operator!= (const Service &that) const
    {
        return !(*this == that);
    }     
    
}