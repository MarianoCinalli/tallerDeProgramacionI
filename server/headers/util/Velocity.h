#include "util/Constants.h"
#include <cmath>
#include <string>
#include "interfaces/Loggable.h"

#ifndef VELOCITY_H
#define VELOCITY_H

class Velocity : public Loggable {
    private:
        float x;
        float y;
    public:
        Velocity(int x, int y);
        Velocity(float x, float y);
        void accelerate(int direction, float amount);
        void accelerate(int direction);
        void decelerate(int direction, int amount);
        float getComponentX();
        float getComponentY();
        void set(Velocity* velocity);
        void setComponentX(int x);
        void setComponentY(int y);
        void setFloatX(float x);
        void setFloatY(float y);
        void scale(float num);
        void scaleY(int num);
        void scaleX(int num);
        void decelerate(int amount);
        bool isZero();
        int getAsOrientation();
        void normalize();
        float distanceTo(Velocity *other);

            void stop();
        void stopDirection(int direction);
        std::string toString();
        ~Velocity();

    private:
        void changeOnDirection(int direction, float amount);
        void changeOnDirection(int direction);
};
#endif // VELOCITY_H
