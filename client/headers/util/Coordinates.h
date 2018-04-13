#include <string>
#include "interfaces/Loggable.h"
#include "util/logger.h"

#ifndef COORDINATES_H
#define COORDINATES_H

class Coordinates : public Loggable {
    private:
        int x;
        int y;
        void validatePositionInBounds();

    public:
        Coordinates(int x, int y);
        int getX();
        int getY();
        void setX(int otherX);
        void setY(int otherY);

        void addX(int amount);
        void subtractX(int amount);

        void addY(int amount);
        void subtractY(int amount);

        void setCoord(Coordinates other);
        int distanceTo(Coordinates* other);
        std::string toString();

        ~Coordinates();
};
#endif // COORDINATES_H
