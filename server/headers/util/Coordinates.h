#include <string>
#include "interfaces/Loggable.h"
#include "util/logger.h"

#ifndef COORDINATES_H
#define COORDINATES_H

class Coordinates : public Loggable {
    private:
        float x;
        float y;

    public:
        // Coordinates(int x, int y);
        Coordinates(float x, float y);
        float getX();
        float getY();
        int setX(float otherX);
        int setY(float otherY);

        void normalize();
        int addX(float amount);
        int addY(float amount);
        int validatePositionInBoundsY();
        int validatePositionInBoundsX();
        bool isThisPosition(Coordinates* pos);
        void set(Coordinates other);
        float distanceTo(Coordinates* other);
        std::string toString();

        ~Coordinates();
};
#endif // COORDINATES_H
