#include <string>
#include "interfaces/Loggable.h"
#include "util/logger.h"

#ifndef COORDINATES_H
#define COORDINATES_H

class Coordinates : public Loggable {
    private:
        int x;
        int y;

    public:
        Coordinates(int x, int y);
        int getX();
        int getY();
        int setX(int otherX);
        int setY(int otherY);

        int addX(int amount);
        void subtractX(int amount);

        int addY(int amount);
        void subtractY(int amount);

        int validatePositionInBounds();
        void set(Coordinates other);
        int distanceTo(Coordinates* other);
        std::string toString();

        ~Coordinates();
};
#endif // COORDINATES_H
