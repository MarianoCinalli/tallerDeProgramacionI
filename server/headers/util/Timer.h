#include "util/Constants.h"
#include "util/logger.h"
#include "util/Time.h"
#include <chrono>

#ifndef TIMER_H
#define TIMER_H

class Timer {
    private:
        std::chrono::system_clock::time_point startTimePoint;
        std::chrono::system_clock::time_point stopTimePoint;
        bool started;
        float scaleFactor;
    public:
        Timer(int timePerHalf);
        void start();
        void resume();
        bool hasStarted();
        Time* getTime();
        void stop();
        ~Timer();
};
#endif // TIMER_H
