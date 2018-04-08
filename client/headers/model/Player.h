#include "util/Constants.h"
#include "util/Coordinates.h"
#include "util/Velocity.h"
#include "util/logger.h"
#include <SDL2/SDL.h>

#ifndef PLAYER_H
#define PLAYER_H

class Player {
    private:
        int orientation;
        int maxVelocity;
        Velocity* velocity;
        Coordinates* position;

    public:
        Player(int orientation, Coordinates* position);

        // Getters
        Coordinates* getPosition();
        Velocity* getVelocity();
        int getOrientation();

        // Modifiers
        // Cambia la velocidad a su maximo, por ahora.
        void accelerate(int direction);
        void decelerate(int direction);
        // Cambia la posicion con respecto a su velocidad.
        void updatePosition();
        void setOrientation(int orientation);
        void setPosition(Coordinates coord);

        ~Player();
};
#endif // PLAYER_H
