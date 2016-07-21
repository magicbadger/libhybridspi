#include <hybridspi/services.h>
#include <sstream>

using namespace std;
using namespace hybridspi;

ProgrammeInfo create_programmeinfo()
{
    auto tp = make_datetime(2014, 4, 11, 1, 20, 0, 0, 1);

    ProgrammeInfo info;

    Schedule schedule(tp);
    schedule.SetOriginator("Global Radio");

    // programme
    Programme programme("crid://www.capitalfm.com/4772/1190223", 1190223);
    programme.AddName(ShortName("B'fast"));
    programme.AddName(MediumName("Breakfast"));
    programme.AddName(LongName("Capital Breakfast"));

    // location
    Location location;
    location.AddAbsoluteTime(AbsoluteTime(make_datetime(2014, 4, 25, 6, 0, 0, 0, 1), Duration(60 * 60 * 4)));
    programme.AddLocation(location);

    // descriptions
    programme.AddDescription(ShortDescription("Forget the coffee, Capital gives you the perfect morning pick-me- up with a blend of the latest hits, travel news and incomparable morning banter."));

    // genres
    programme.AddGenre(Genre(string("urn:tva:metadata:cs:ContentCS:2002:3.6.8")));
    programme.AddGenre((string("urn:tva:metadata:cs:IntentionCS:2002:1.1")));

    // memberships
    programme.AddMembership(Membership("crid://www.capitalfm.com/4772", 4772));

    // links
    programme.AddLink(Link("mailto:capital.breakfast@capitalfm.com"));
    programme.AddLink(Link("http://www.capitalfm.com/on-air/breakfast-show/"));

    // programme event
    ProgrammeEvent event("crid://thisisglobal.com/4772/1190223/788946", 788946);
    event.AddName(ShortName("Pun"));
    event.AddName(MediumName("No.1 Pun"));
    event.AddName(LongName("London's No. 1 Pun"));
    event.AddDescription(ShortDescription("Can you come up with London\'s No.1 Pun for our story of the day?"));
    Location eventLocation;
    eventLocation.AddRelativeTime(RelativeTime(Duration(60*60*3 + 60*10), Duration(60*25)));
    event.AddLocation(eventLocation);
    programme.AddEvent(event);

    schedule.AddProgramme(programme);
    info.AddSchedule(schedule);
    return info;
}
