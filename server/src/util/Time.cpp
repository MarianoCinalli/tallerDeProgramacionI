#include "util/Time.h"

Time::Time(std::chrono::duration<double> interval) {
    this->interval = interval;
}

// Devuelve los segundos pasados. Sin los minutos. Ver toString() como se usa.
int Time::getSeconds() {
    return (this->getSecondsCount()) % 60;
}

// Devuelve los minutos pasados. Sin los segundos. Ver toString() como se usa.
int Time::getMinutes() {
    return ((int) floor(this->interval.count() / 60));
}

// Devuelve Minutos-Segundos
std::string Time::toString() {
    return this->getMinutesAsString() + "-" + this->getSecondsAsString();
}

// Devuelve el tiempo que paso en segundos.
int Time::getSecondsCount() {
    return (int) floor(this->interval.count());
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
