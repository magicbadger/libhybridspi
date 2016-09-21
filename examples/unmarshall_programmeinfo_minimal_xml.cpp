#include <hybridspi/services.h>
#include <hybridspi/xml.h>
#include <hybridspi/util.h>

#include <iostream>
#include <fstream>

using namespace hybridspi;
using namespace hybridspi::xml;

int main()
{
    cout << "reading file" << endl;
    ifstream ifs("examples/PI_minimal.xml");
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
        }
        cout << endl;
    }

    return 0;
}
