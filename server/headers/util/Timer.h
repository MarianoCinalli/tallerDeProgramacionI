#include "util/Constants.h"
#include "util/logger.h"
#include "util/Time.h"
#include <chrono>

#ifndef TIMER_H
#define TIMER_H

class Timer {
    private:
        std::chrono::system_clock::time_point startTimePoint;
    public:
        Timer();
        void start();
        Time* getTime();
        void stop();
        ~Timer();
};
#endif // TIMER_H
