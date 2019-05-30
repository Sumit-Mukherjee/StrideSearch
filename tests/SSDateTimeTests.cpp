#include "StrideSearchConfig.h"
#include "SSDefs.hpp"
#include "SSUtilities.hpp"
#include "SSDateTime.hpp"

#include <iostream>

using namespace StrideSearch;

int main (int argc, char* argv[]) {
    print_copyright();
    
    int year = 2017;
    int month = 1;
    int day = 18;
    int hour = 9;
    DateTime codingDay(year, month, day, hour);
    std::cout << "DateTime::DTGString() = " << codingDay.DTGString("T") << std::endl;
    std::cout << "DateTime::intString() = " << codingDay.intString() << std::endl;
        
    DateTime codingDay2(year, month, day, hour);
    std::cout << "(True) Equivalent date-times? " << (codingDay == codingDay2 ? "true" : "false") << std::endl;
    if (codingDay != codingDay2) {  
        throw std::logic_error("the impossible happened.");
    }
    
    codingDay2 = DateTime(year, month, day, hour + 1);
    std::cout << "(False) Equivalent date-times? " << (codingDay == codingDay2 ? "true" : "false") << std::endl;
    
    std::cout << "(False) codingDay2 < codingDay1? " << (codingDay2 < codingDay ? "true" : "false") << std::endl;
    if (codingDay2 < codingDay) {
        throw std::logic_error("the impossible happened.");
    }
    
    DateTime codingDay3("2017-01-18-09");
    std::cout << "DateTime::DTGString() = " << codingDay3.DTGString("T") << std::endl;
    
    DateTime codingDay4("2017-01-18");
    std::cout << "DateTime::DTGString() = " << codingDay4.DTGString("T") << std::endl;
    
    const DateTime startDate(2000, 1, 1, 0);
    DateTime relativeDate(300.25, startDate);
    std::cout << "300.25 days after Jan-1-2000 is " << relativeDate.DTGString() << std::endl;
    
    const DateTime startDate2(2017,12, 15, 0);
    const std::vector<Real> day_incs = {15, 16, 17, 17.25, 365, 365.5};
    for (int i=0; i<day_incs.size(); ++i) {
        const DateTime relDate(day_incs[i], startDate2);
        std::cout << day_incs[i] << " days after midnight, Dec-15-2017, is " << relDate.easyReadStr() << '\n';
    }
    
    const DateTime simStart(1851,10,1,0);
    const std::vector<Real> tvals = {330, 330.25, 330.5, 330.75, 331};
    for (int i=0; i<tvals.size(); ++i) {
        const DateTime relDate(tvals[i], simStart);
        std::cout << tvals[i] << " days after " << simStart.easyReadStr() << ", is " << relDate.easyReadStr() << '\n';
        std::cout << "\tISO STD 8601 format: " << relDate.isoFullStr() << ", or " << relDate.isoDatetimeStr() << '\n';
        if (i==0) {
            if (relDate != DateTime(1852,8,27,0)) {
                throw std::logic_error("bad relative date computation.");
            }
        }
    }
    
std::cout << "tests pass." << std::endl;
return 0;
}
