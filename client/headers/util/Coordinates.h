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

        void incrementX();
        void decrementX();

        void incrementY();
        void decrementY();

        ~Coordinates();
};
#endif // COORDINATES_H


