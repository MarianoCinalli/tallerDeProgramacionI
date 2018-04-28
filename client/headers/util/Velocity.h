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
        void setComponentX(int x);
        void setComponentY(int y);
        bool isZero();
        
        void stop();
        void stopDirection(int direction);
        std::string toString();
        ~Velocity();

    private:
        void changeOnDirection(int direction, int amount);
};
#endif // VELOCITY_H
