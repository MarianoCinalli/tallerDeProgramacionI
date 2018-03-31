#include "util/Constants.h"
#include "util/Coordinates.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player {
    private:
        int orientation;
        int maxVelocity;
        Velocity* velocity;
        Coordinates* position;
        SpriteManager* spriteManager;

    public:
        Player(int orientation, Coordinates* position);

        // Getters
        Coordinates* getPosition();
        int getOrientation();

        // Modifiers
        void accelerate(int direction);
        void decelerate(int direction);
        void setOrientation(int orientation);

        void render(SDL_Renderer* screen, Coordinates* position);

        ~Player();
};
#endif // PLAYER_H
