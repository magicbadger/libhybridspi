#ifndef HYBRIDSPI_PROGRAMMES_H
#define HYBRIDSPI_PROGRAMMES_H

#include <hybridspi/common.h>

using namespace std;
using namespace hybridspi;

namespace hybridspi
{

    class Location
    {
        public:

           
        private:

            vector<Bearer*> bearers; 

            vector<RelativeTime> relativeTimes;

            vector<Time> absoluteTimes;

    }

    class Located
    {
        public:

            Located();

            vector<Location> Locations() const { return locations; };

            void AddLocation(Location location);

            void RemoveLocation(const Location &location);

        private:

            vector<Location> locations;
    };

    class Membershipped
    {
        public:
        
            Membershipped();

            vector<Membership> Memberships() const { return memberships; };

            void AddMembership(Membership membership);

            void RemoveMembership(const Membership &membership);
    }

    class ProgrammeEvent : public Named, public Described, public Linked, public Genred, public MediaEnabled, 
                           public Keyworded, public Located, public Membershipped
    {
        public:

            ProgrammeEvent(string id, int shortId, version = 1, recommendation = false);

        private:

            unsigned int version;
        
            bool recommendation;
    }

    class Programme : public Named, public Described, public Linked, public Genred, public MediaEnabled, 
                      public Keyworded, public Located, public Membershipped
    {
        public:
            
            Programme(string id, int shortId, version = 1, recommendation = false);

        private:

            vector<ProgrammeEvent> events;

            unsigned int version;
        
            bool recommendation;

    };

    class Schedule
    {
        public:
            
            Schedule(DateTime start, DateTime end, DateTime created = system_clock::now(), unsigned int version = 0);

            void SetOriginator(string originator);

            void AddProgramme(Programme programme);

            vector<Programme> Programmes() const { return programmes; }

            void RemoveProgramme(const Programme &programme);

        private:

            DateTime start;
            
            DateTime end;

            DateTime created;

            unsigned int version;

            string originator;

            vector<Programme> programmes;
    };

    class ProgrammeInfo
    {
        public:
            
            ProgrammeInfo();

            void AddSchedule(Schedule schedule);

            vector<Schedule> Schedules() const { return schedules; }

            void RemoveSchedule(const Schedule &schedule);

        private:

            vector<Schedule> schedules;
        
    };
}

#endif
