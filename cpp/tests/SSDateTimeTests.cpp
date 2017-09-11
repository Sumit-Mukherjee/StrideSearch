#include "StrideSearchConfig.h"
#include "StrideSearchTypeDefs.h"
#include "StrideSearchUtilities.h"
#include "StrideSearchDateTime.h"

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
    
    codingDay2 = DateTime(year, month, day, hour + 1);
    std::cout << "(False) Equivalent date-times? " << (codingDay == codingDay2 ? "true" : "false") << std::endl;
    
    std::cout << "(False) codingDay2 < codingDay1? " << (codingDay2 < codingDay ? "true" : "false") << std::endl;
    
    DateTime codingDay3("2017-01-18-09");
    std::cout << "DateTime::DTGString() = " << codingDay3.DTGString("T") << std::endl;
    
    DateTime codingDay4("2017-01-18");
    std::cout << "DateTime::DTGString() = " << codingDay4.DTGString("T") << std::endl;
    
    const DateTime startDate(2000, 1, 1, 0);
    DateTime relativeDate(300.25, startDate);
    std::cout << "300.25 days after Jan-1-2000 is " << relativeDate.DTGString() << std::endl;
return 0;
}
