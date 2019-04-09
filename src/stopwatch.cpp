#include <stopwatch.h>
//#include <chrono>

class stopwatch
{
    std::chrono::time_point<std::chrono::high_resolution_clock> time_reference;
    public:
    stopwatch()
    {
        reset();
    }
    void reset()
    {
        time_reference = std::chrono::high_resolution_clock::now();
    }
    double seconds()
    {
        auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-time_reference);
        double elapsed = static_cast<double>(d.count())/1000000000;
        return elapsed;
    }
    long long abs_time_LongLong()
    {
        auto t = std::chrono::high_resolution_clock::now().time_since_epoch();
        auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(t);
        long long elapsed = d.count();
        return elapsed;
    }
};
