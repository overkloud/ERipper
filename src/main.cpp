#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <random>
#include <functional>
#include <sstream>
#include <time.h>
//#include <libxml/parser.h>
//#include <libxml/tree.h>
//#include <libxml/HTMLParser.h>
#include <curl/curl.h>

//#include "xmlutil.h"
#include "util.h"

using namespace std;

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  int written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int scrap(string ticker, string outfile)
{
  CURL *curl_handle;
  //static const char *headerfilename = "head.out";
  //string headerfilename = ticker + ".header.out";
  //FILE *headerfile;
  //static const char *bodyfilename = "body.out";
  string bodyfilename = outfile;
  FILE *bodyfile;

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */ 
  curl_handle = curl_easy_init();

  const string url ="http://finviz.com/quote.ashx?t=" + ticker;

  /* set URL to get */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

  /* no progress meter please */ 
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

  /* open the header file */ 
//   headerfile = fopen(headerfilename.c_str(), "wb");
//   if(!headerfile) {
//     curl_easy_cleanup(curl_handle);
//     return -1;
// }

  /* open the body file */ 
  bodyfile = fopen(bodyfilename.c_str(), "wb");
  if(!bodyfile) {
    curl_easy_cleanup(curl_handle);
    //fclose(headerfile);
    return -1;
  }

  /* we want the headers be written to this file handle */ 
//curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, headerfile);

  /* we want the body be written to this file handle instead of stdout */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, bodyfile);

  /* get it! */ 
  curl_easy_perform(curl_handle);

  /* close the header file */ 
//fclose(headerfile);

  /* close the body file */ 
  fclose(bodyfile);

  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
  return 0;
}


CDate getFinvizEarning(string filename)
{
    //xmlDoc *doc = NULL;
    //doc = xmlReadFile(file.c_str(), NULL, 0);
    //xmlNodePtr root = getRootElement(parseXml(file));

//    htmlDocPtr doc = htmlReadFile(file.c_str(),  "ISO-8859-1", 0);//

//    if(doc == NULL)
//    {
//        cout << "cannot open the file " << file << endl;
//    }
  ifstream file(filename);

  file.seekg(0, file.end);
  int size = file.tellg();
  file.seekg(0, file.beg);
  if(!file.good())
  {
    cout << " file is bad " << endl;
  }

  char * data = new char [size];
  file.read(data, size);
  cout << "file read: " << size << endl;

  
  //const string endings [] = {"AMC", "BMO"};

  string s(std::move(data));

  std::regex erdata ("Earnings date");
  std::smatch mdata;
  if(std::regex_search(s, mdata, erdata))
  {
    s = mdata.suffix().str();
    string ssm = "(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)\\s\\d*\\s(AMC|BMO)";
    //cout << ssm << endl;
    std::regex er(ssm);
    std::smatch m;
    if (std::regex_search(s, m, er))
    {
      for (auto x:m) 
      {
        //cout << x << endl;
        //er = x;
        stringstream ss;
        ss << x;
        string m_str;
        int d;// d_str;
        int y;
        ss >> m_str;
        ss >> d;
        time_t now;
        struct tm* now_yr;
        time(&now);
        now_yr =localtime(&now);
        y = 1900 + now_yr->tm_year;


        //int m;
        std::function<int(string &)> convertMonth = 
        [](string &m)
        {
            //date er;
            const map<string, int> months = {
            {"Jan", 1},
            {"Feb", 2},
            {"Mar", 3},
            {"Apr", 4},
            {"May", 5},
            {"Jun", 6},
            {"Jul", 7},
            {"Aug", 8},
            {"Sep", 9},
            {"Oct", 10},
            {"Nov", 11},
            {"Dec", 12}
            };
            for (auto a : months)
            {
                if(a.first == m) return a.second;
            }

            throw string("cannot find match month for " + m);
            return -1;
        };
        int m = convertMonth(m_str);
        //int d = atoi(d_str.c_str());


        return (CDate(y, m, d));
      }
    }
    else
    {
      cout << "nothing matches" << endl;
    }
    // for (auto mn : months)
    // {
    //     for(auto ending : endings)
    //     {
    //         stringstream ss;
    //         ss << mn << "\\s\\d*\\s" << ending;
    //     //cout << ss.str() << endl;
    //         std::regex er( ss.str());
    //         std::smatch m;
    //         if(std::regex_search(s, m, er))
    //         {
    //             for(auto x:m) cout << x << " ";
    //             break;
    //         }
    //     }
    // }


  }
  else
  {
    cout << "No matching data for earning" << endl;
  }

  //std::regex month("<b>Apr");
  //CDate c;
  return CDate();

}


vector<string> getTickers(const string filename)
{
    //char ticker [256];
  ifstream infile (filename);
  vector<string> ret;
  if(infile.good())
  {
    while(infile.eof() == false)
    {
            //infile.getline(ticker, sizeof(ticker)/sizeof(char));
      string ticker;
      std::getline(infile, ticker);
      if(ticker.empty() == false) ret.emplace_back(ticker);
    }
  }
  else
  {
    throw string (filename + " is no good");
  }

  return ret;
}

int printHelp(const string program_name)
{
    cout << program_name << " -i ticker_list " << 
      "-d date -w work dir -b basefile " << endl;//-o output directory <<
    return 0;  
}

enum DIFF_TYPE
{
    ADD = 0,
    REMOVE = 1,
    DIFF = 2

};

typedef map<string, pair<DIFF_TYPE, int>> DiffResult;

DiffResult diffTwoFiles(string file1, string file2)
{
    ifstream f1(file1);
    ifstream f2(file2);

    DiffResult ret;

    if(f1.good() == false) 
    {
        cout << "file " << file1 << " is bad" << endl;
        return ret;
    }

    if(f2.good() == false)
    {
        cout << "file " << file2 << " is bad" << endl;
        return ret;
    }


    std::function<void(ifstream &, map<string,string>&)> 
    loadDatafile= [](ifstream &f1, 
        map<string, string> & m)
    {
        string line;
        while(f1.eof() == false)
        {
          getline(f1, line);
          size_t pos = line.find(",");
          if(pos == string::npos) continue;
          string ticker = line.substr(0, pos);
          string date = line.substr(pos + 1);
            //cout << ticker << endl;
            //cout << date << endl;
          m.emplace(ticker, date);
        }
    };
    //map<string, string> src;
    //string line;
    map<string, string> ref;
    map<string, string> test;
    loadDatafile(f1, ref);
    loadDatafile(f2, test);

    for(auto a : test)
    {
        //cout << a.first << ":" << a.second << endl;
        auto it = ref.find(a.first);
        if(it != ref.end())
        {
            if(it->second != a.second)
            {
                CDate refDate(it->second);
                CDate newDate(a.second);
                int diff = newDate - refDate;
                ret.emplace(it->first, make_pair(DIFF, diff));
            }
            //cout << it->first << ":" << it->second << endl;
        }
    }

    //for(auto a : m2)
    //    cout << a.first << ":" << a.second << endl;

    for(auto it : ret)
    {
        //char c = it.second.first == DIFF ? '+' : '-';
        cout << it.first << ": " << it.second.second << endl;
    }
    return ret;
}

string dumpER(string wdir, string d, vector<string>&tickers)
{

    string sumfile (d);
    if(wdir.empty() == false)
    {
        sumfile = wdir + "/" + sumfile;
    }
    ofstream sumout (sumfile);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch()
    .count();
    std::minstd_rand0 ranGen(seed);
    for(auto s : tickers)
    {
        cout << "getting " << s << endl;
        string filename = s + "." + d + "." + "raw";
        if(wdir.empty() == false)
        {
          filename = wdir + "/" + filename;
      }
      scrap(s, filename);
      unsigned int sleeptime = ranGen() % 5001;
      cout << "sleeping " << sleeptime << endl;
      this_thread::sleep_for(chrono::milliseconds(sleeptime));               
      CDate er = getFinvizEarning(filename);
      sumout <<  s + "," + er.toString() << endl;
    }
    sumout.close();

    return sumfile;
}

int main(int argc, char** argv)
{
    //parse("body.out"); 

    //char file [204800];
    //getFinvizEarning("body.out");
    string ticker_list = "";
    string date="";
    string wdir;
    string basefile;
    int count = argc;

    if(argc == 1)
    {
        return printHelp(argv[0]);  
    }

    for(int i = 1; i < argc; i++)
    {
        string s (argv[i]);
        if(s == "-i")  
        {
            ticker_list.assign(argv[i + 1]);
        }
        else if (s == "-d")
        {
          date.assign(argv[i + 1]);
        }
        else if(s == "-w")
        {
          wdir.assign(argv[i + 1]);
        }
        else if(s == "-b")
        {
          basefile.assign(argv[i + 1]);
        }
    }

    if(date.empty() || ticker_list.empty())
        return printHelp(argv[0]);

    vector<string> tickers = getTickers(ticker_list);
    std::sort(tickers.begin(), tickers.end());
    for(string s : tickers)
        cout << s << endl;

    //string sumfile = dumpER(wdir, date, tickers);
    string sumfile = wdir + "/20150816";

    diffTwoFiles(basefile, sumfile);
    return 0;
}
