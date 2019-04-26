// stopwatch is a class that uses the chrono library to perform a stopwatch function.

#ifndef STOPWATCH
#define STOPWATCH

#include <chrono>

class stopwatch
{
    std::chrono::time_point<std::chrono::high_resolution_clock> time_reference;
    public:
    stopwatch();
    void reset();
    double seconds();
    long long abs_time_LongLong();
};

#endif // STOPWATCH
