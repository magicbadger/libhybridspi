#ifndef HYBRIDSPI_PROGRAMMES_H
#define HYBRIDSPI_PROGRAMMES_H

#include <hybridspi/common.h>

using namespace std;
using namespace hybridspi;

namespace hybridspi
{
        
    class AbsoluteTime
    {
        public:
        
            AbsoluteTime(DateTime billedTime, Duration billedDuration);
            
            AbsoluteTime(DateTime billedTime, Duration billedDuration, DateTime actualTime, Duration actualDuration);
            
            DateTime BilledTime() { return billedTime; };
            
            Duration BilledDuration() { return billedDuration; };
            
            DateTime ActualTime() { return actualTime; };
            
            Duration ActualDuration() { return actualDuration; };
            
            bool operator== (const AbsoluteTime &that) const;
            
            bool operator!= (const AbsoluteTime &that) const;               

        private:

            DateTime billedTime, actualTime;
            Duration billedDuration, actualDuration;
            
    };

    class RelativeTime
    {
        public:
        
            RelativeTime(Duration billedTime, Duration billedDuration);
            
            RelativeTime(Duration billedTime, Duration billedDuration, Duration actualTime, Duration actualDuration);
            
            Duration BilledTime() { return billedTime; };
            
            Duration BilledDuration() { return billedDuration; };
            
            Duration ActualTime() { return actualTime; };
            
            Duration ActualDuration() { return actualDuration; };
            
            bool operator== (const RelativeTime &that) const;
            
            bool operator!= (const RelativeTime &that) const;               

        private:

            Duration billedTime, billedDuration, actualTime, actualDuration;
        
    };

    class Location
    {
        public:
        
            Location();
            
            vector<Bearer*> Bearers() const { return bearers; };

            void AddBearer(Bearer* bearer);

            void RemoveBearer(const Bearer* bearer);
            
            vector<RelativeTime> RelativeTimes() const { return relativeTimes; };

            void AddRelativeTime(const RelativeTime &relativeTime);

            void RemoveRelativeTime(const RelativeTime &relativeTime);        
            
            vector<AbsoluteTime> AbsoluteTimes() const { return absoluteTimes; };

            void AddAbsoluteTime(const AbsoluteTime &absoluteTime);

            void RemoveAbsoluteTime(const AbsoluteTime &absoluteTime);      
            
            bool operator== (const Location &that) const;
            
            bool operator!= (const Location &that) const;                            
           
        private:

            vector<Bearer*> bearers; 

            vector<RelativeTime> relativeTimes;

            vector<AbsoluteTime> absoluteTimes;

    };

    class Located
    {
        public:

            Located();

            vector<Location> Locations() const { return locations; };

            void AddLocation(const Location &location);

            void RemoveLocation(const Location &location);

        private:

            vector<Location> locations;
    };

    class Membershipped
    {
        public:
        
            Membershipped();

            vector<Membership> Memberships() const { return memberships; };

            void AddMembership(const Membership &membership);

            void RemoveMembership(const Membership &membership);

        private:
            
            vector<Membership> memberships;
    };

    class ProgrammeEvent : public Named, public Described, public Linked, public Genred, public MediaEnabled, 
                           public Keyworded, public Located, public Membershipped
    {
        public:

            ProgrammeEvent(string id, int shortId, int version = 1, bool recommendation = false);

            string Id() { return id; };
            
            unsigned int ShortId() { return shortId; };
            
            unsigned short Version() { return version; };            
            
            bool operator== (const ProgrammeEvent &that) const;
            
            bool operator!= (const ProgrammeEvent &that) const;               

        private:
        
            string id;
            
            unsigned int shortId;

            unsigned int version;
        
            bool recommendation;
    };

    class Programme : public Named, public Described, public Linked, public Genred, public MediaEnabled, 
                      public Keyworded, public Located, public Membershipped
    {
        public:
            
            Programme(string id, unsigned int shortId, unsigned short version = 1, bool recommendation = false);
            
            string Id() { return id; };
            
            unsigned int ShortId() { return shortId; };
            
            unsigned short Version() { return version; };
            
            vector<ProgrammeEvent> Events() { return events; };

            void AddEvent(const ProgrammeEvent &event);

            void RemoveEvent(const ProgrammeEvent &event);
            
            bool operator== (const Programme &that) const;
            
            bool operator!= (const Programme &that) const;               

        private:
        
            string id;
            
            unsigned int shortId;

            unsigned short version;
        
            bool recommendation;

            vector<ProgrammeEvent> events;

    };

    class Schedule
    {
        public:
            
            Schedule(DateTime created = system_clock::now(), unsigned int version = 0);
            
            DateTime Created() { return created; };
            
            unsigned int Version() { return version; };
            
            void SetScope(DateTime start, DateTime stop);
            
            pair<DateTime, DateTime> Scope();

            void SetOriginator(string originator);
            
            string Originator() { return originator; };

            void AddProgramme(const Programme &programme);

            vector<Programme> Programmes() const { return programmes; }

            void RemoveProgramme(const Programme &programme);
            
            bool operator== (const Schedule &that) const;
            
            bool operator!= (const Schedule &that) const;               

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

            void AddSchedule(const Schedule &schedule);

            vector<Schedule> Schedules() const { return schedules; }

            void RemoveSchedule(const Schedule &schedule);

        private:

            vector<Schedule> schedules;
        
    };
}

#endif
