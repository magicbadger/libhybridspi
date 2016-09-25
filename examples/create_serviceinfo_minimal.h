#include <hybridspi/services.h>
#include <sstream>

using namespace std;
using namespace hybridspi;

ServiceInfo create_serviceinfo()
{
    tm t = {};
    stringstream ss("2015-09-21T06:30:15");
    ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
    auto epoch = mktime(&t);
    auto tp = std::chrono::system_clock::from_time_t(epoch);

    ServiceInfo info(tp);
    
    // sample service
    Service service("www.heart.co.uk", "bristol");

    // names
    service.AddName(ShortName("Heart"));
    service.AddName(MediumName("Heart Bristol"));
    service.AddName(LongName("Heart Bristol"));

    // descriptions
    service.AddDescription(ShortDescription(string("More Music Variety")));

    // logos (3 local, 1 remote)
    service.AddMedia(Multimedia("32x32.png", "image/png", 32, 32));
    service.AddMedia(Multimedia("112x32.png", "image/png", 112, 32));
    service.AddMedia(Multimedia("128x128.png", "image/png", 128, 128));
    service.AddMedia(Multimedia("http://owdo.thisisglobal.com/2.0/id/44/logo/320x240.jpg", "image/png", 320, 240));

    // bearers
    DabBearer* dabBearer = new DabBearer(0xe1, 0xc181, 0xc36b, 0x0, "audio/mpeg", 20, 2500);
    dabBearer->SetBitrate(128000);
    service.AddBearer(dabBearer);
    IpBearer* ipBearer = new IpBearer("http://media-ice.musicradio.com/HeartBristol", "audio/aacp", 70, 16000);
    ipBearer->SetBitrate(48000);
    service.AddBearer(ipBearer);

    // links
    service.AddLink(Link("http://www.heart.co.uk/bristol"));
    service.AddLink(Link("mailto:bristol@heart.co.uk"));

    info.AddService(service);
    return info;
}
