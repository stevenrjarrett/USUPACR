#include "stopwatch.h"
//#include <chrono>


stopwatch::stopwatch()
{
    reset();
}

// resets the stopwatch (saves the time reference as right now.)
void stopwatch::reset()
{
    time_reference = std::chrono::high_resolution_clock::now();
}

// returns the number of seconds since the reference time
double stopwatch::seconds()
{
    auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-time_reference);
    double elapsed = static_cast<double>(d.count())/1000000000;
    return elapsed;
}

// returns the time since epoch
long long stopwatch::abs_time_LongLong()
{
    auto t = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(t);
    long long elapsed = d.count();
    return elapsed;
}
