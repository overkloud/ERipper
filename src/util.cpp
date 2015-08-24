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

CDate::CDate(const string & s)
{
    this->y = atoi(s.substr(0, 4).c_str());
    this->m = atoi(s.substr(4, 2).c_str());
    this->d = atoi(s.substr(6, 2).c_str());
    //using namespace std;
    //cout << y << m << d << endl;;
}
