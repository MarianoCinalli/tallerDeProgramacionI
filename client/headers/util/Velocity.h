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
        void accelerate(int direction);
        void decelerate(int direction, int amount);
        int getComponentX();
        int getComponentY();
        float totalVelocity();
        void set(Velocity* velocity);
        void setComponentX(int x);
        void setComponentY(int y);
        bool isZero();
        int getAsOrientation();
        void scale(int num);
        void decelerate(int amount);
        void stop();
        void stopDirection(int direction);
        std::string toString();
        ~Velocity();

    private:
        void changeOnDirection(int direction, int amount);
        void changeOnDirection(int direction);
};
#endif // VELOCITY_H
