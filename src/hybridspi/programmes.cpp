#include <hybridspi/programmes.h>
#include <iostream>

AbsoluteTime::AbsoluteTime(DateTime billedTime, Duration billedDuration)
    : billedTime(billedTime), billedDuration(billedDuration), actualTime(billedTime), actualDuration(billedDuration)
{ };
            
AbsoluteTime::AbsoluteTime(DateTime billedTime, Duration billedDuration, DateTime actualTime, Duration actualDuration)
    : billedTime(billedTime), billedDuration(billedDuration), actualTime(actualTime), actualDuration(actualDuration)
{ };

bool AbsoluteTime::operator== (const AbsoluteTime &that) const
{
    return (billedTime == that.billedTime &&
            billedDuration == that.billedDuration &&
            actualTime == that.actualTime &&
            actualDuration == that.actualDuration);
}
    
bool AbsoluteTime::operator!= (const AbsoluteTime &that) const
{
    return !(*this == that);
}  
 
RelativeTime::RelativeTime(Duration billedTime, Duration billedDuration)
    : billedTime(billedTime), billedDuration(billedDuration), actualTime(billedTime), actualDuration(billedDuration)
{ };
            
RelativeTime::RelativeTime(Duration billedTime, Duration billedDuration, Duration actualTime, Duration actualDuration)
    : billedTime(billedTime), billedDuration(billedDuration), actualTime(actualTime), actualDuration(actualDuration)
{ };

bool RelativeTime::operator== (const RelativeTime &that) const
{
    return (billedTime == that.billedTime &&
            billedDuration == that.billedDuration &&
            actualTime == that.actualTime &&
            actualDuration == that.actualDuration);
}
    
bool RelativeTime::operator!= (const RelativeTime &that) const
{
    return !(*this == that);
}  

Location::Location()
{ }

void Location::AddAbsoluteTime(const AbsoluteTime &absoluteTime)
{
    absoluteTimes.push_back(absoluteTime);
}

void Location::RemoveAbsoluteTime(const AbsoluteTime &absoluteTime)
{
    auto it = std::find(absoluteTimes.begin(), absoluteTimes.end(), absoluteTime);
    if(it != absoluteTimes.end())
        absoluteTimes.erase(it);    
}

void Location::AddRelativeTime(const RelativeTime &relativeTime)
{
    relativeTimes.push_back(relativeTime);
}

void Location::RemoveRelativeTime(const RelativeTime &relativeTime)
{
    auto it = std::find(relativeTimes.begin(), relativeTimes.end(), relativeTime);
    if(it != relativeTimes.end())
        relativeTimes.erase(it);    
}

void Location::AddBearer(Bearer* bearer)
{
    bearers.push_back(bearer);
}

void Location::RemoveBearer(const Bearer* bearer)
{
    auto it = std::find(bearers.begin(), bearers.end(), bearer);
    if(it != bearers.end())
        bearers.erase(it);    
}
    
bool Location::operator== (const Location &that) const
{
    
    if(this->bearers != that.bearers) return false;
    if(this->absoluteTimes.size() > 0)
    {
        if(this->absoluteTimes != that.absoluteTimes) return false;
    }
    if(this->relativeTimes.size() > 0)
    {
        if(this->relativeTimes != that.relativeTimes) return false;
    }
    return true;
}
    
bool Location::operator!= (const Location &that) const
{
    return !(*this == that);
}

Schedule::Schedule(DateTime created, unsigned int version)
    : created(created), version(version)
{ }
    
void Schedule::AddProgramme(const Programme &programme)
{
    programmes.push_back(programme);
}           

void Schedule::SetScope(DateTime start, DateTime end)
{
    this->start = start;
    this->end = end;
}

pair<DateTime, DateTime> Schedule::Scope()
{
    DateTime undefined = system_clock::from_time_t(0);
    
    // we have a defined scope
    if(this->start > undefined && this->end > undefined)
    {
        return pair<DateTime, DateTime>(this->start, this->end);
    }
    
    // calculate the scope from the programmes
    DateTime a, b = undefined;
    for(auto &programme : programmes)
    {
        for(auto &location : programme.Locations())
        {
            // programmes have absolute times
            for(auto time : location.AbsoluteTimes())
            {
                DateTime timeStart = time.ActualTime();
                DateTime timeEnd = timeStart + time.ActualDuration();
                if(a == undefined || timeStart < a)
                {
                    a = DateTime(timeStart);
                }
                if(b == undefined || timeEnd > b)
                {
                    b = DateTime(timeEnd);
                }
            }
        }
    }
    
    return pair<DateTime, DateTime>(a, b);
}

void Schedule::SetOriginator(string originator)
{
    this->originator = originator;
}

Programme::Programme(string id, unsigned int shortId, unsigned short version, bool recommendation)
    : id(id), shortId(shortId), version(version), recommendation(recommendation), 
      Named(), Described(), Linked(), Genred(), MediaEnabled(), Keyworded(), Membershipped()
{ }

void Programme::AddEvent(const ProgrammeEvent &event)
{
    events.push_back(event);
}

void Programme::RemoveEvent(const ProgrammeEvent &event)
{
    auto it = std::find(events.begin(), events.end(), event);
    if(it != events.end())
        events.erase(it);       
}

Membershipped::Membershipped()
{ }

void Membershipped::AddMembership(const Membership &membership)
{
    memberships.push_back(membership);
}

void Membershipped::RemoveMembership(const Membership &membership)
{
    auto it = std::find(memberships.begin(), memberships.end(), membership);
    if(it != memberships.end())
        memberships.erase(it);   
}

ProgrammeInfo::ProgrammeInfo()
{ }

void ProgrammeInfo::AddSchedule(const Schedule &schedule)
{
    schedules.push_back(schedule);
}

void ProgrammeInfo::RemoveSchedule(const Schedule &schedule)
{
    auto it = std::find(schedules.begin(), schedules.end(), schedule);
    if(it != schedules.end())
        schedules.erase(it);   
}

ProgrammeEvent::ProgrammeEvent(string id, int shortId, int version, bool recommendation)
    : id(id), shortId(shortId), version(version), recommendation(recommendation)
{ }

Located::Located()
{ }

void Located::AddLocation(const Location &location)
{
    locations.push_back(location);
}

void Located::RemoveLocation(const Location &location)
{
    auto it = std::find(locations.begin(), locations.end(), location);
    if(it != locations.end())
        locations.erase(it);      
}
