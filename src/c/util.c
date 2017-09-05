#include "util.h"

struct tm* get_time_now()    {
    // get a 'tm' structure
    time_t time_standard = time(NULL);
    struct tm *tick_time = localtime(&time_standard);    
    
    return tick_time;
}