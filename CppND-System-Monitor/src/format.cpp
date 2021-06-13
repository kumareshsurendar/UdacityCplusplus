#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: duration in seconds
// OUTPUT: time in hours:minute:seconds format
string Format::ElapsedTime(long seconds) {
    char ch_timeFormat[10];
    int i_hour = seconds / 3600;
    int residue = seconds % 3600;
    int i_min = residue / 60;
    int i_sec = residue % 60;
    sprintf(ch_timeFormat, "%.2d:%.2d:%.2d", i_hour, i_min, i_sec);
    return ch_timeFormat;
}