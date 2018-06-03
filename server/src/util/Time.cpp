#include "util/Time.h"

Time::Time(std::chrono::duration<double> interval) {
    this->interval = interval;
}

int Time::getSeconds() {
    return ((int) floor(this->interval.count())) % 60;
}

int Time::getMinutes() {
    return ((int) floor(this->interval.count() / 60));
}

std::string Time::toString() {
    return this->getMinutesAsString() + "-" + this->getSecondsAsString();
}

std::string Time::getSecondsAsString() {
    int seconds = this->getSeconds();
    std::string secondsString = std::to_string(seconds);
    if (seconds < 10) {
        secondsString += "0";
    }
    return secondsString;
}

std::string Time::getMinutesAsString() {
    int minutes = this->getMinutes();
    std::string minutesString = std::to_string(minutes);
    if (minutes < 10) {
        minutesString = "0" + minutesString;
    }
    return minutesString;
}

Time::~Time() {
}
