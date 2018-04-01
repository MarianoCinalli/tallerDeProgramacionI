#ifndef COORDINATES_H
#define COORDINATES_H

class Coordinates {
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

        ~Coordinates();
};
#endif // COORDINATES_H


