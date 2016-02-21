#ifndef HYBRIDSPI_MARSHALL_H
#define HYBRIDSPI_MARSHALL_H

namespace hybridspi
{
    class Marshaller
    {
        public :
        
            virtual vector<unsigned char> Marshall(ServiceInfo service_info) const = 0;
            
            // virtual Marshall(ProgrammeInfo programme_info) = 0;
            
            // virtual Marshall(GroupInfo group_info) = 0;
    };
}

#endif