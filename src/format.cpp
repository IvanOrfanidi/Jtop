#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    int h = seconds/3600;
    int m = (seconds / 60) % 60;
    int s = seconds % 60;
    return IntToStrFormat(h) + ":" + IntToStrFormat(m) + ":" + IntToStrFormat(s);
 }

 string Format::IntToStrFormat(int n){
     std::stringstream ss;
     ss << std::setw(2) << std::setfill('0') << n;
     return ss.str();
 }