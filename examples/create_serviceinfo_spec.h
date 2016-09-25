#include <hybridspi/services.h>
#include <sstream>

using namespace std;
using namespace hybridspi;

ServiceInfo create_serviceinfo()
{
    tm t;
    stringstream ss("2014-04-25T00:05:31+01:00");
    ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
    auto epoch = mktime(&t);
    auto tp = std::chrono::system_clock::from_time_t(epoch);

    ServiceInfo info(tp);
      
    // sample service
    Service service("www.capitalfm.com", "london");

    // names
    service.AddName(ShortName("Capital"));
    service.AddName(MediumName("Capital FM"));
    service.AddName(LongName("Capital London"));

    // descriptions
    service.AddDescription(ShortDescription("The UK's No.1 Hit Music Station"));
    
    // links
    Link smsLink("sms:83958");
    smsLink.SetDescription("Text the Studio");
    service.AddLink(smsLink);
    service.AddLink(Link("http://www.capitalfm.com/london", "text/html"));

    // logos (3 local, 1 remote)
    service.AddMedia(Multimedia("http://owdo.thisisglobal.com/2.0/id/25/logo/32x32.png", "image/png", 32, 32));
    service.AddMedia(Multimedia("http://owdo.thisisglobal.com/2.0/id/25/logo/112x32.png", "image/png", 112, 32));
    service.AddMedia(Multimedia("http://owdo.thisisglobal.com/2.0/id/25/logo/128x128.png", "image/png", 128, 128));
    service.AddMedia(Multimedia("http://owdo.thisisglobal.com/2.0/id/25/logo/320x240.jpg", "image/png", 320, 240));
    service.AddMedia(Multimedia("http://owdo.thisisglobal.com/2.0/id/25/logo/600x600.jpg", "image/png", 600, 600));
    service.AddMedia(Multimedia("http://owdo.thisisglobal.com/2.0/id/25/logo/1024x768.jpg", "image/png", 1024, 768));


    // genres
    service.AddGenre(Genre("urn:tva:metadata:cs:ContentCS:2004:3.6.10"));
    service.AddGenre(Genre("urn:tva:metadata:cs:ContentCS:2004:3.6.8"));
    service.AddGenre(Genre("urn:tva:metadata:cs:ContentCS:2004:3.1.1.11"));
    service.AddGenre(Genre("urn:tva:metadata:cs:ContentCS:2004:3.6.8.14"));
    service.AddGenre(Genre("urn:tva:metadata:cs:ContentCS:2004:3.1.4.12"));
    
    // bearers
    service.AddBearer(new DabBearer(0xe1, 0xc185, 0xc479, 0x0, "audio/mpeg", 20, 2000));
    service.AddBearer(new FmBearer(0xe1, 0xc479, 95800, 30));
    IpBearer *ipBearer1 = new IpBearer("http://media-ice.musicradio.com/Capital", "audio/aacp", 40, 4000);
    ipBearer1->SetBitrate(48);
    service.AddBearer(ipBearer1);
    IpBearer *ipBearer2 = new IpBearer("http://media-ice.musicradio.com/CapitalMP3Low", "audio/mpeg", 40, 4000);
    ipBearer2->SetBitrate(48);
    service.AddBearer(ipBearer2);
    
    // keywords
    service.AddKeywords("London, music, pop, rock, dance, urban");

    info.AddService(service);
    
    info.SetOriginator("Global");
    
    return info;
}
