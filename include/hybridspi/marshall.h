#ifndef HYBRIDSPI_MARSHALL_H
#define HYBRIDSPI_MARSHALL_H

#include <vector>
#include <hybridspi/services.h>
#include <hybridspi/programmes.h>
#include <hybridspi/groups.h>

using namespace std;
using namespace hybridspi;

namespace hybridspi
{
    class Marshaller
    {
        public :
        
            virtual vector<unsigned char> Marshall(ServiceInfo service_info) const = 0;
            
            virtual vector<unsigned char> Marshall(ProgrammeInfo programme_info) const = 0;
            
            virtual vector<unsigned char> Marshall(GroupInfo group_info) const = 0;
            
            virtual ServiceInfo UnmarshallServiceInfo(vector<unsigned char> bytes) const = 0;
            
            virtual ProgrammeInfo UnmarshallProgrammeInfo(vector<unsigned char> bytes) const = 0;
            
            virtual GroupInfo UnmarshallGroupInfo(vector<unsigned char> bytes) const = 0;

    };
}

#endif