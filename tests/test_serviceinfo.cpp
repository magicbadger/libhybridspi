#include <hybridspi/services.h>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace hybridspi;

int main()
{

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    cout << std::put_time(std::localtime(&now_c), "%F %T") << '\n';
    
    Service service1("First Service");
    Service service2("Second Service");
    Service service3("Third Service");
    ServiceInfo info;
    info.AddService(service1);
    info.AddService(service2);
    info.AddService(service3);
    
    return 0;
}