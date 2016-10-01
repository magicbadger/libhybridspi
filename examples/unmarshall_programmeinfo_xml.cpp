#include <hybridspi/xml.h>
#include <hybridspi/util.h>

#include <iostream>
#include <fstream>

using namespace hybridspi;
using namespace hybridspi::xml;

int main(int argc, char* argv[])
{
    cout << "Reading from: " << argv[0] << endl;
    ifstream ifs(argv[1]);
    string content((istreambuf_iterator<char>(ifs)),
                   (istreambuf_iterator<char>()));
    cout << "Read " << content.size() << " bytes"  << endl;

    XmlMarshaller marshaller;
    vector<unsigned char> data(content.begin(), content.end());
    ProgrammeInfo info = marshaller.UnmarshallProgrammeInfo(data);

    cout << info.Schedules().size() << " schedules" << endl;
    for(auto &schedule : info.Schedules())
    {
        cout << "Schedule with " << schedule.Programmes().size() << " Programmes: originator=" << schedule.Originator() << ", created=" << schedule.Created() << endl;
        for(auto &programme : schedule.Programmes())
        {
            cout << "\tProgramme: id=" << programme.Id() << ", shortId=" << programme.ShortId() << endl;
            cout << "\t\tNames: " << join<Name>(programme.Names()) << endl;
            cout << "\t\tDescriptions: " << join<Description>(programme.Descriptions()) << endl;
            cout << "\t\tLinks: " << join<Link>(programme.Links()) << endl;
            cout << "\t\tGenres: " << join<Genre>(programme.Genres()) << endl;
            cout << "\t\tMedia: " << join<Multimedia>(programme.Media()) << endl;
            cout << "\t\tMembership: " << join<Membership>(programme.Memberships()) << endl;
            cout << "\t\tLocations: " << join<Location>(programme.Locations()) << endl;
        }
        cout << endl;
    }

    return 0;
}
