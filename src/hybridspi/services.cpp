#include <hybridspi/services.h>

namespace hybridspi
{
    ServiceProvider::ServiceProvider()
    { }
   
    ServiceInfo::ServiceInfo()
        : created(system_clock::now()), version(0)
    { }
    
    ServiceInfo::ServiceInfo(DateTime created, int version)
        : created(created), version(version)
    { }
    
    // ServiceInfo::ServiceInfo(DateTime created, int version, string originator, ServiceProvider provider)
    //     : created(created), version(version), originator(originator), provider(provider)
    // { }
    
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
        time_t time = system_clock::to_time_t(info.GetCreated());
        string txt("ServiceInfo: services=");
        os << txt;
        return os;
    }
    
    Service::Service(string lookup)
        : lookup(lookup), Named(), Described(), GeoLocated(), Linked(), Genred(), MultimediaEnable(), Keyworded() 
    { }
    
    bool Service::operator== (const Service &that) const
    {
        return lookup == that.lookup;
    }
            
    bool Service::operator!= (const Service &that) const
    {
        return !(*this == that);
    }     
    
}