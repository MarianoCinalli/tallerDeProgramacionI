#include <string>
#include "interfaces/Loggable.h"

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
