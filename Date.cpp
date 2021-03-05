#include <iostream>
#include <sstream>
#include "Date.hpp"
using namespace std;

bool valid_str(const std::string &t_dateString) {
    if((t_dateString[4] != '-') || (t_dateString[7] != '-') 
        || (t_dateString[10] != '/') || (t_dateString[13] != ':') || (t_dateString.length() != 16))
        return false;

    for(int i = 0; i < 4; ++i)
    {
        if((t_dateString[i] < '0') || (t_dateString[i] > '9'))
            return false;
    }
    for(int i = 5; i < 7; ++i)
    {
        if((t_dateString[i] < '0') || (t_dateString[i] > '9'))
            return false;
    }
    for(int i = 8; i < 10; ++i)
    {
        if((t_dateString[i] < '0') || (t_dateString[i] > '9'))
            return false;
    }
    for(int i = 11; i < 13; ++i)
    {
        if((t_dateString[i] < '0') || (t_dateString[i] > '9'))
            return false;
    }
    for(int i = 14; i < 16; ++i)
    {
        if((t_dateString[i] < '0') || (t_dateString[i] > '9'))
            return false;
    }
    return true;
}

Date::Date() {
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
    m_year = t_year;
    m_month = t_month;
    m_day = t_day;
    m_hour = t_hour;
    m_minute = t_minute;
}

Date::Date(const std::string &dateString) {
    *this = this->stringToDate(dateString);
}

int Date::getYear(void) const {
    return m_year;
}

void Date::setYear(const int t_year) {
    m_year = t_year;
}

int Date::getMonth(void) const {
    return m_month;
}

void Date::setMonth(const int t_month) {
    m_month = t_month;
}

int Date::getDay(void) const {
    return m_day;
}

void Date::setDay(const int t_day) {
    m_day = t_day;
}

int Date::getHour(void) const {
    return m_hour;
}   

void Date::setHour(const int t_hour) {
    m_hour = t_hour;
}

int Date::getMinute(void) const {
    return m_minute;
}

void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
}

bool Date::isValid(const Date &t_date) {
    if((t_date.getYear() < 1000) || (t_date.getYear() > 9999) || (t_date.getMonth() <= 0) || (t_date.getMonth() > 12)
        || (t_date.getDay() <= 0) || (t_date.getDay() > 31) || (t_date.getHour() < 0) || (t_date.getHour() > 23)
        || (t_date.getMinute() < 0) || (t_date.getMinute() > 59))
        return false;
    if(t_date.getMonth() == 4 || t_date.getMonth()== 6 || t_date.getMonth() == 9 ||  t_date.getMonth() == 11)
        if(t_date.getDay() == 31)
            return false;
    if((t_date.getMonth() == 2) && (t_date.getDay() > 29))
        return false;
    if(!((t_date.getYear() % 400 == 0) || ((t_date.getYear() % 4 == 0) && (t_date.getYear() % 100 != 0))))
        if((t_date.getMonth() == 2) && (t_date.getDay() == 29))
            return false;
    return true;
}

Date Date::stringToDate(const std::string &t_dateString) {
    if(!valid_str(t_dateString)) 
    {
        Date validtemp;
        return validtemp;
    }
    int tt_year, tt_month, tt_day, tt_hour, tt_minute;

    string t_year = t_dateString.substr(0,4);
    stringstream s1(t_year);
    s1 >> tt_year;

    string t_month = t_dateString.substr(5,2);
    stringstream s2(t_month);
    s2 >> tt_month;

    string t_day = t_dateString.substr(8,2);
    stringstream s3(t_day);
    s3 >> tt_day;

    string t_hour = t_dateString.substr(11,2);
    stringstream s4(t_hour);
    s4 >> tt_hour;

    string t_minute = t_dateString.substr(14,2);
    stringstream s5(t_minute);
    s5 >> tt_minute;

    Date temp(tt_year, tt_month, tt_day, tt_hour, tt_minute);
    return temp;
}

std::string Date::dateToString(const Date &t_date) {
    if(!isValid(t_date))
        return "0000-00-00/00:00";

    string outcome;

    stringstream s1;
    s1 << t_date.getYear();
    outcome.append(s1.str());

    outcome.append("-");

    if(t_date.getMonth() < 10)
        outcome.append("0");
    stringstream s2;
    s2 << t_date.getMonth();
    outcome.append(s2.str());

    outcome.append("-");

    if(t_date.getDay() < 10)
        outcome.append("0");
    stringstream s3;
    s3 << t_date.getDay();
    outcome.append(s3.str());

    outcome.append("/");

    if(t_date.getHour() < 10)
        outcome.append("0");
    stringstream s4;
    s4 << t_date.getHour();
    outcome.append(s4.str());

    outcome.append(":");

    if(t_date.getMinute() < 10)
        outcome.append("0");
    stringstream s5;
    s5 << t_date.getMinute();
    outcome.append(s5.str());

    return outcome;
}

Date &Date::operator=(const Date &t_date) {
    this->m_year = t_date.getYear();
    this->m_month = t_date.getMonth();
    this->m_day = t_date.getDay();
    this->m_hour = t_date.getHour();
    this->m_minute = t_date.getMinute();
    return *this;
}

bool Date::operator==(const Date &t_date) const {
    if((this->m_year == t_date.getYear()) && (this->m_month == t_date.getMonth()) &&
        (this->m_day == t_date.getDay()) && (this->m_hour == t_date.getHour()) &&
        (this->m_minute == t_date.getMinute()))
        return true;
    return false;
}

bool Date::operator>(const Date &t_date) const {
    if(this->m_year > t_date.getYear())
        return true;
    else if(this->m_year == t_date.getYear())
        if(this->m_month > t_date.getMonth())
            return true;
        else if(this->m_month == t_date.getMonth())
            if(this->m_day > t_date.getDay())
                return true;
            else if(this->m_day == t_date.getDay())
                if(this->m_hour > t_date.getHour())
                    return true;
                else if(this->m_hour == t_date.getHour())
                    if(this->m_minute > t_date.getMinute())
                        return true;

    return false;
}

bool Date::operator<(const Date &t_date) const {
    if(*this >= t_date)                      ///////////
        return false;
    return true;
}

bool Date::operator>=(const Date &t_date) const {
    if(this->m_year > t_date.getYear())
        return true;
    else if(this->m_year == t_date.getYear())
        if(this->m_month > t_date.getMonth())
            return true;
        else if(this->m_month == t_date.getMonth())
            if(this->m_day > t_date.getDay())
                return true;
            else if(this->m_day == t_date.getDay())
                if(this->m_hour > t_date.getHour())
                    return true;
                else if(this->m_hour == t_date.getHour())
                    if(this->m_minute > t_date.getMinute())
                        return true;
                        else if(this->m_minute == t_date.getMinute())
                            return true;

    return false;
}

bool Date::operator<=(const Date &t_date) const {
    if(*this > t_date)
        return false;
    return true;
}
