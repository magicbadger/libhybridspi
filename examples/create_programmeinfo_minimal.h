#include <hybridspi/services.h>
#include <sstream>

using namespace std;
using namespace hybridspi;

ProgrammeInfo create_programmeinfo()
{
    ProgrammeInfo info;

    Schedule schedule;

    // programme
    Programme programme("crid://www.capitalfm.com/4772/1190223", 1190223);
    programme.AddName(MediumName("Breakfast"));

    // location
    Location location;
    auto tp = make_datetime(2014, 4, 25, 6, 0, 0, 0, 1);
    location.AddAbsoluteTime(AbsoluteTime(tp, Duration(60 * 60 * 4)));
    programme.AddLocation(location);

    // descriptions
    programme.AddDescription(ShortDescription("Forget the coffee, Capital gives you the perfect morning pick-me- up with a blend of the latest hits, travel news and incomparable morning banter."));

    schedule.AddProgramme(programme);
    info.AddSchedule(schedule);
    return info;
}
