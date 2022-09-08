#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{   int h = int(seconds / 3600);
    int m = int(seconds % 3600)/60;
    int s = int(seconds % 3600)%60;

    string sec = std::to_string(s);
    if(sec.length()<2)sec.insert(0, 2 - sec.length(), '0');
    
    string mins = std::to_string(m);
    if(mins.length()<2)mins.insert(0, 2 - mins.length(), '0');

    string hrs = std::to_string(h);
    if(hrs.length()<2)hrs.insert(0, 2 - hrs.length(), '0');
        
    return hrs + ":" + mins + ":" +sec; 
    //return std::to_string(h) + ":" + std::to_string(m) + ":" +std::to_string(s); 
}