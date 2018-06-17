#include <chrono>
#include <string>
#include <string.h>
#include <math.h>

#ifndef TIME_H
#define TIME_H

class Time {
    private:
        std::chrono::duration<double> interval;
        float scaleFactor;
    public:
        Time(std::chrono::duration<double> interval);
        Time(std::chrono::duration<double> interval, float scaleFactor);
        int getSeconds();
        int getMinutes();
        std::string toString();
        std::string getSecondsAsString();
        std::string getMinutesAsString();
        int getSecondsCount();
        ~Time();
};
#endif // TIME_H


