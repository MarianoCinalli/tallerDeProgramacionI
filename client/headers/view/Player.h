#include "util/Constants.h"
#include "util/Coordinates.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player {
    private:
        int orientation;
        Coordinates* position;
        Sprite* sprite;

    public:
        Player(int orientation, Coordinates* position);

        // Getters
        Coordinates* getPosition();
        int getOrientation();

        // Modifiers
        void moveUp();
        void moveDown();
        void moveRight();
        void moveLeft();
        void setOrientation(int orientation);

        void render(SDL_Renderer* screen, Coordinates* position);

        ~Player();
};
#endif // PLAYER_H
