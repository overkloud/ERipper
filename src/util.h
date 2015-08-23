#include <string>
#include <vector>
#include <sstream>

#include <ctime>
#include <functional>


using namespace std;

class CDate
{
    public:
        CDate(int y, int m, int d);

        CVDate(const string & s);

        CDate()
        {
            //invalid = true;
        };

        int operator- (const CDate & d)
        {
            //return this->serial - d.serial;
            time_t this_tm = dateToTimeT(this->m, this->d, this->y);
            time_t d_tm = dateToTimeT(d.m, d.d, d.y);

            double sec = difftime(this_tm, d);
            return sec / (60 * 60 * 24);
        }

        string toString()
        {
            stringstream ss;
            ss << y;
            if(m < 10) ss << "0";
            ss << m;
            if(d < 10) ss << "0";
            ss << d;
            return ss.str();
        }

    private:
        time_t dateToTimeT(int month, int day, int year) {
          // january 5, 2000 is passed as (1, 5, 2000)
          tm tmp = tm();
          tmp.tm_mday = day;
          tmp.tm_mon = month - 1;
          tmp.tm_year = year - 1900;
          return mktime(&tmp);
        }


        int serial;
        bool valid = false;
        int y;
        int m;
        int d;
};