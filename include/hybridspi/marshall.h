#ifndef HYBRIDSPI_MARSHALL_H
#define HYBRIDSPI_MARSHALL_H

#include <vector>
#include <hybridspi/services.h>

using namespace std;
using namespace hybridspi;

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