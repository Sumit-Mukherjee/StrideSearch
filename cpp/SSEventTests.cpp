#include "StrideSearchUtilities.h"
#include "StrideSearchEvent.h"
#include <iostream>
#include <string>
#include <vector>

typedef std::pair<double, double> ll_coord_type;
typedef std::vector<int> indices_type;

int main (int argc, char* argv[]) {
    print_copyright();
    
    const int timestep_size_hours = 6;
    DateTime dt1(2017, 1, 18, 0);
    DateTime dt2(2017, 1, 18, timestep_size_hours);
    
    ll_coord_type loc1(45.0, 0.0);
    ll_coord_type loc2(45.2, 0.2);
    
    indices_type index1 = {1,1};
    indices_type index2 = {1,2};
    
    const double ps1val = 990.0;
    const double ps2val = 992.0;
    const int time_index = 0;
    const std::string fname = "fakeFile.nc";
    const double radius_km = 500.0;
    Event ev_ps1("min(PSL)", ps1val, loc1, dt1, index1, fname, time_index, Event::Min);
    std::cout << ev_ps1.infoString();
    
    Event ev_ps2("min(PSL)", ps2val, loc2, dt1, index2, fname, time_index, Event::Min);
    std::cout << "ev_ps2 is duplicate of ev_ps1? " << (ev_ps1.isDuplicate(ev_ps2) ? "true" : "false") << std::endl;
    std::cout << "ev_ps2 is near ev_ps1? " << (ev_ps1.isNear(ev_ps2, radius_km) ? "true" : "false") << std::endl;
    std::cout << "ev_ps2 is less intense than ev_ps1? " << (ev_ps2 < ev_ps1 ? "true" : "false") << std::endl;
    std::cout << "ev_ps2 is redundant listing of ev_ps1? " << 
        (ev_ps2.isRedundant(ev_ps1, radius_km) ? "true" : "false") << std::endl;
    
    const double vor1val = 0.0035;
    Event ev_vor1("max(VOR)", vor1val, loc2, dt1, index2, fname, time_index, Event::Max);
    
    ev_ps1.addRelated(&ev_vor1);
    std::cout << ev_ps1.infoString();
return 0;
}