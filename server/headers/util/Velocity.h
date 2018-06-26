#include "util/Constants.h"
#include <string>
#include <cmath>
#include "interfaces/Loggable.h"

#ifndef VELOCITY_H
#define VELOCITY_H

class Velocity : public Loggable {
    private:
        float x;
        float y;
    public:
        Velocity(float x, float y);
        void accelerate(int direction, int amount);
        void accelerate(int direction);
        void decelerate(int direction, int amount);
        int getComponentX();
        int getComponentY();
        float getFloatX();
        float getFloatY();
        void set(Velocity* velocity);
        void setComponentX(int x);
        void setComponentY(int y);
        void setFloatX(float x);
        void setFloatY(float y);
        void scale(int num);
        void scaleY(int num);
        void scaleX(int num);
        void decelerate(float amount);
        bool isZero();
        int getAsOrientation();

        void normalize();
        void stop();
        void stopDirection(int direction);
        std::string toString();
        ~Velocity();

    private:
        void changeOnDirection(int direction, int amount);
        void changeOnDirection(int direction);
};
#endif // VELOCITY_H
