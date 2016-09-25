#include <hybridspi/services.h>
#include <hybridspi/xml.h>
#include <hybridspi/util.h>

#include <iostream>
#include <fstream>

using namespace hybridspi;
using namespace hybridspi::xml;

int main(int argc, char* argv[])
{
    cout << "reading from: " << argv[0] << endl;
    ifstream ifs(argv[1]);
    string content((istreambuf_iterator<char>(ifs)),
                   (istreambuf_iterator<char>()));
    cout << "read " << content.size() << " bytes"  << endl;

    XmlMarshaller marshaller;
    vector<unsigned char> data(content.begin(), content.end());
    ProgrammeInfo info = marshaller.UnmarshallProgrammeInfo(data);

    for(auto &schedule : info.Schedules())
    {
        for(auto &programme : schedule.Programmes())
        {
            cout << "====================================================================" << endl;
            cout << "Programme: id=" << programme.Id() << ", shortId=" << programme.ShortId() << endl;
            for(auto &name : programme.Names())
            {
                cout << name.Text() << endl;
            }
            for(auto &description : programme.Descriptions())
            {
                cout << description.Text() << endl;
            }
            for(auto &link : programme.Links())
            {
                cout << link << endl;
            }
            for(auto &genre : programme.Genres())
            {
                cout << genre << endl;
            }
            for(auto &media : programme.Media())
            {
                cout << media << endl;
            }
            for(auto &membership : programme.Memberships())
            {
                cout << membership.Id() << endl;
            }
            for(auto &location : programme.Locations())
            {
                for(auto *bearer : location.Bearers())
                {
                    cout << bearer->URI() << endl;
                }
            }
        }
        cout << endl;
    }

    return 0;
}
