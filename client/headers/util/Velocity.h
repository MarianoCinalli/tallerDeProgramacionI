#include "util/Constants.h"
#include <string>
#include "interfaces/Loggable.h"

#ifndef VELOCITY_H
#define VELOCITY_H

class Velocity : public Loggable {
    private:
        int x;
        int y;
    public:
        Velocity(int x, int y);
        void accelerate(int direction, int amount);
        void decelerate(int direction, int amount);
        int getComponentX();
        int getComponentY();
        bool isZero();
        void stop();
        std::string toString();
        ~Velocity();

    private:
        void changeOnDirection(int direction, int amount);
};
#endif // VELOCITY_H
