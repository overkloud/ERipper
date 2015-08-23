#include "util.h"


CDate::CDate(int y, int m, int d)
{
    valid = true;
    serial = 365 * (y - 1900) + 
            30 * (m - 1) +
            d;

    this->y = y;
    this->m = m;
    this->d = d;
}

CVDate::CVDate(const string & s)
{
    int y = atoi(s.substr(0, 4).c_str());
    int m = atoi(s.substr(4, 2).c_str());
    int d = atoi(s.substr(6, 2).c_str());
}
