#include "util/Timer.h"

Timer::Timer(int timePerHalf) {
    log("Timer: Creando el timer...", LOG_INFO);
    this->started = false;
    this->startTimePoint = std::chrono::system_clock::now();
    this->scaleFactor = MINUTES_PER_HALF / timePerHalf;
}

void Timer::start() {
    log("Timer: Iniciando el timer.", LOG_INFO);
    this->startTimePoint = std::chrono::system_clock::now();
    this->started = true;
}

// Devuelve el tiempo pasado desde que se inicio el timer.
Time* Timer::getTime() {
    if (this->started){
    std::chrono::system_clock::time_point nowTimePoint = std::chrono::system_clock::now();
    return new Time(nowTimePoint - this->startTimePoint, scaleFactor);
  }
  else{
    return new Time(std::chrono::system_clock::now() - std::chrono::system_clock::now());
  }
}

bool Timer::hasStarted() {
    return this->started;
}

void Timer::stop() {
    log("Timer: Frenando el timer.", LOG_INFO);
}

Timer::~Timer() {
    log("Timer: Borrando el timer.", LOG_INFO);
}
