#include <chrono>
#include <string>
#include <string.h>
#include <math.h>

#ifndef TIME_H
#define TIME_H

class Time {
    private:
        std::chrono::duration<double> interval;
    public:
        Time(std::chrono::duration<double> interval);
        int getSeconds();
        int getMinutes();
        std::string toString();
        std::string getSecondsAsString();
        std::string getMinutesAsString();
        ~Time();
};
#endif // TIME_H


