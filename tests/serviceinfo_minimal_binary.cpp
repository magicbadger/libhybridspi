#include <hybridspi/services.h>
#include <hybridspi/binary.h>
#include <hybridspi/util.h>
#include <sstream>

#include "test_util.h"

using namespace hybridspi;
using namespace hybridspi::binary;

int main()
{
    tm t = {};
    stringstream ss("2015-09-21T06:30:15");
    ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
    auto epoch = mktime(&t);
    auto tp = std::chrono::system_clock::from_time_t(epoch);

    ServiceInfo info(tp);
    
    // sample service
    Service service("http://www.heart.co.uk/bristol");

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
    service.AddBearer(new DabBearer(0xe1, 0xc181, 0xc36b, 0x0, 128, "audio/mpeg", 20, 2500));
    service.AddBearer(new IpBearer("http://media-ice.musicradio.com/HeartBristol", 48, "audio/aacp", 70, 16000));

    // links
    service.AddLink(Link("http://www.heart.co.uk/bristol"));
    service.AddLink(Link("mailto:bristol@heart.co.uk"));
    
    // add the service
    info.AddService(service);
    
    // ensemble - just for binary
    Ensemble ensemble(0xe1, 0xc181);

    BinaryMarshaller marshaller(ensemble);
    vector<unsigned char> bytes = marshaller.Marshall(info);

    ASSERT("serviceinfo minimal binary", bytes_to_hex(bytes), "03 FE 01 4B 81 06 37 F1 89 9E 3C 00 26 FE 01 3F 80 03 E1 C1 81 28 FE 01 36 29 08 80 06 40 E1 C1 81 C3 6B 29 2E 82 2C 68 74 74 70 3A 2F 2F 6D 65 64 69 61 2D 69 63 65 2E 6D 75 73 69 63 72 61 64 69 6F 2E 63 6F 6D 2F 48 65 61 72 74 42 72 69 73 74 6F 6C 10 07 01 05 48 65 61 72 74 11 0F 01 0D 48 65 61 72 74 20 42 72 69 73 74 6F 6C 12 0F 01 0D 48 65 61 72 74 20 42 72 69 73 74 6F 6C 13 16 1A 14 01 12 4D 6F 72 65 20 4D 75 73 69 63 20 56 61 72 69 65 74 79 13 20 2B 1E 80 09 69 6D 61 67 65 2F 70 6E 67 82 09 33 32 78 33 32 2E 70 6E 67 84 02 00 20 84 02 00 20 13 21 2B 1F 80 09 69 6D 61 67 65 2F 70 6E 67 82 0A 31 31 32 78 33 32 2E 70 6E 67 84 02 00 20 84 02 00 70 13 22 2B 20 80 09 69 6D 61 67 65 2F 70 6E 67 82 0B 31 32 38 78 31 32 38 2E 70 6E 67 84 02 00 80 84 02 00 80 13 4E 2B 4C 80 09 69 6D 61 67 65 2F 70 6E 67 82 37 68 74 74 70 3A 2F 2F 6F 77 64 6F 2E 74 68 69 73 69 73 67 6C 6F 62 61 6C 2E 63 6F 6D 2F 32 2E 30 2F 69 64 2F 34 34 2F 6C 6F 67 6F 2F 33 32 30 78 32 34 30 2E 6A 70 67 84 02 00 F0 84 02 01 40")
}
