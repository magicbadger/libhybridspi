#include <hybridspi/services.h>
#include <hybridspi/binary.h>
#include <hybridspi/util.h>
#include <iostream>

using namespace std;
using namespace hybridspi;
using namespace hybridspi::binary;

int main()
{
    ServiceInfo info;
    
    // sample service
    Service service("http://www.heart.co.uk/bristol");

    // names
    service.AddName(ShortName("Heart"));
    service.AddName(MediumName("Heart Bristol"));
    service.AddName(LongName("Heart Bristol"));

    // descriptions
    //service.AddDescription(ShortDescription(string("More Music Variety")));

    // logos (3 local, 1 remote)
    service.AddMedia(Multimedia("32x32.png", "image/png", 32, 32));
    service.AddMedia(Multimedia("112x32.png", "image/png", 112, 32));
    service.AddMedia(Multimedia("128x128.png", "image/png", 128, 128));
    service.AddMedia(Multimedia("http://owdo.thisisglobal.com/2.0/id/44/logo/320x240.jpg", "image/png", 320, 240));

    // bearers
    service.AddBearer(DabBearer(0xe1, 0xc181, 0xc36b, 0x0, 128, "audio/mpeg", 20, 2500));
    service.AddBearer(IpBearer("http://media-ice.musicradio.com/HeartBristol", 48, "audio/aacp", 70, 16000));

    info.AddService(service);
    
    // ensemble
    Ensemble ensemble(0xe1, 0xc181);

    BinaryMarshaller marshaller(ensemble);
    vector<unsigned char> bytes = marshaller.Marshall(info);
    cout << bytes.size() << " bytes" << endl;
    cout << bytes_to_hex(bytes) << endl;

    return 1;
}
