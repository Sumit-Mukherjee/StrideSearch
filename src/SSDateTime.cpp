#include "SSDateTime.hpp"
#include "SSConsts.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <exception>


namespace StrideSearch {

std::tm DateTime::dt2tm() const {
    std::tm result = {0};
    result.tm_sec = 0;
    result.tm_min = 0;
    result.tm_hour = hour;
    result.tm_mday = day;
    result.tm_mon = month-1;
    result.tm_year = year - 1900;
    result.tm_isdst = -1;
    return result;
}

DateTime::DateTime(const int yr, const int mo, const int dy, const int hr) : year(yr), month(mo), day(dy), hour(hr) {
    if (monthDayMap.empty()) 
        buildMonthDayMap();
}

DateTime::DateTime(const std::string ymd_string) {
    year = std::stoi(ymd_string.substr(0,4));
    month = std::stoi(ymd_string.substr(5,2));
    day = std::stoi(ymd_string.substr(8,2));
    std::string hr_str = "00";
    try {
         hr_str = ymd_string.substr(11,2);
    }
    catch (const std::out_of_range& e) {}
    hour = std::stoi(hr_str);
    if (monthDayMap.empty())
        buildMonthDayMap();
}

DateTime::DateTime(const std::tm& ctm) : year(ctm.tm_year), month(ctm.tm_mon), day(ctm.tm_mday), hour(ctm.tm_hour) {
    if (monthDayMap.empty()) 
        buildMonthDayMap();
};

DateTime::DateTime(const Real daysSinceStart, const DateTime& start) {
    if (monthDayMap.empty()) 
        buildMonthDayMap();
    
    std::tm stm = start.dt2tm();    
    std::time_t date_seconds = std::mktime(&stm) + SIDEREAL_DAY_SEC * daysSinceStart;
    std::tm date = *std::localtime(&date_seconds);
    year = date.tm_year+1900;
    month = date.tm_mon+1;
    day = date.tm_mday;
    hour = date.tm_hour;
}

void DateTime::buildMonthDayMap() {
    monthDayMap.emplace(1, 31);
    monthDayMap.emplace(2, 28);
    monthDayMap.emplace(3, 31);
    monthDayMap.emplace(4, 30);
    monthDayMap.emplace(5, 31);
    monthDayMap.emplace(6, 30);
    monthDayMap.emplace(7, 31);
    monthDayMap.emplace(8, 31);
    monthDayMap.emplace(9, 30);
    monthDayMap.emplace(10, 31);
    monthDayMap.emplace(11, 30);
    monthDayMap.emplace(12, 31);
    
    monthStringMap.emplace(1, "JAN");
    monthStringMap.emplace(2, "FEB");
    monthStringMap.emplace(3, "MAR");
    monthStringMap.emplace(4, "APR");
    monthStringMap.emplace(5, "MAY");
    monthStringMap.emplace(6, "JUN");
    monthStringMap.emplace(7, "JUL");
    monthStringMap.emplace(8, "AUG");
    monthStringMap.emplace(9, "SEP");
    monthStringMap.emplace(10, "OCT");
    monthStringMap.emplace(11, "NOV");
    monthStringMap.emplace(12, "DEC");
}

std::string DateTime::DTGString(std::string time_zone_string) const {
    std::ostringstream ss4;
    ss4 << std::setw(4) << std::setfill('0') << year;
    std::string yrStr(ss4.str());
    
    std::ostringstream ss2;
    ss2 << std::setw(2) << std::setfill('0') << day;
    std::string dyStr(ss2.str());
    
    ss2.str(std::string());
    ss2 << std::setw(2) << std::setfill('0') << hour << "00";
    std::string hrStr(ss2.str());
    
    std::string moStr;
    try {
     moStr = monthString(month);
    }
    catch (std::exception& e) {
    std::cout << "DateTime::monthString error: caught std::exception " << e.what() << " month = " << month << '\n';
    }

    return dyStr + hrStr + time_zone_string + moStr + yrStr;
}

std::string DateTime::monthString() const {
    return monthStringMap.at(month);
}

std::string DateTime::monthString(const int mInt) const {
    std::string result;
    result = monthStringMap.at(mInt);
    return result;
}


bool operator < (const DateTime& left, const DateTime& right) {
    bool result = false;
    if (left.year < right.year)
        result = true;
    else if (left.year == right.year) {
        if (left.month < right.month) {
            result = true;
        }
        else if (left.month == right.month) {
            if (left.day < right.day) {
                result = true;
            }
            else if (left.day == right.day) {
                if (left.hour < right.hour)
                    result = true;
            }
        }
    }
    return result;
}

bool operator > (const DateTime& left, const DateTime& right) {
    return right < left;
}

std::string DateTime::intString() const{
    std::ostringstream ss;
    ss << std::setw(4) << std::setfill('0') << year;
    ss << std::setw(2) << std::setfill('0') << month;
    ss << std::setw(2) << std::setfill('0') << day;
    ss << std::setw(2) << std::setfill('0') << hour << "00";
    return ss.str();
}

}
