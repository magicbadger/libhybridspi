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
    ifstream ifs("examples/PI_spec.xml");
    string content((istreambuf_iterator<char>(ifs)),
                   (istreambuf_iterator<char>()));
    cout << "read " << content.size() << " bytes"  << endl;

    XmlMarshaller marshaller;
    vector<unsigned char> data(content.begin(), content.end());
    ProgrammeInfo info = marshaller.UnmarshallProgrammeInfo(data);

    for(auto &schedule : info.Schedules())
    {
        /*for(auto &name : service.Names())
        {
            cout << name.Text() << endl;
        }
        for(auto &description : service.Descriptions())
        {
            cout << description.Text() << endl;
        }
        for(auto *bearer : service.Bearers())
        {
            cout << bearer->URI() << endl;
        }
        for(auto &link : service.Links())
        {
            cout << link << endl;
        }
        for(auto &genre : service.Genres())
        {
            cout << genre << endl;
        }
        for(auto &media : service.Media())
        {
            cout << media << endl;
        }
        cout << endl;
        */
    }

    return 0;
}
