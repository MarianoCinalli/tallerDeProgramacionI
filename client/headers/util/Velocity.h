#ifndef VELOCITY_H
#define VELOCITY_H

class Velocity {
    private:
        int x;
        int y;
    public:
        Velocity(int x, int y);
        void accelerate(int direction, int amount);
        void decelerate(int direction, int amount);
        void changeOnDirection(int direction, int amount);
        ~Velocity();
};
#endif // VELOCITY_H


