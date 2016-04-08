#include <hybridspi/services.h>
#include <hybridspi/xml.h>
#include <hybridspi/util.h>

#include <iostream>
#include <sstream>

using namespace std;
using namespace hybridspi;
using namespace hybridspi::xml;

int main(int argc, char* argv[])
{
    vector<unsigned char> bytes;
    for(string line; getline(cin, line);) {
        vector<unsigned char> v(line.begin(), line.end());
        bytes.insert(bytes.end(), v.begin(), v.end());
    }

    printf("%lu bytes retrieved\n", (long)bytes.size());

    XmlMarshaller marshaller;
    cout << "unmarshalling" << endl;
    ServiceInfo info = marshaller.UnmarshallServiceInfo(bytes);
    cout << "unmarshalled" << endl;

    for(auto &service : info.Services())
    {
        stringstream names;
        for(auto &name : service.Names())
        {
            if(names.str().size() > 0) names << ", ";
            names << name.Text();
        }

        cout << names.str() << endl;
        cout << "----------------------" << endl;
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
    }

    return 0;
}
