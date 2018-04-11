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
        bool sliding;
        bool wasSliding;
        bool kicking;
        bool wasKicking;

    public:
        Player(int orientation, Coordinates* position);

        // Getters
        Coordinates* getPosition();
        Velocity* getVelocity();
        int getOrientation();
        bool wasSlidingYet();
        bool wasKickingYet();
        bool isSliding();
        bool isKicking();

        // Modifiers
        // Cambia la velocidad a su maximo, por ahora.
        void accelerate(int direction);
        void decelerate(int direction);
        // Cambia la posicion con respecto a su velocidad.
        void updatePosition();
        void setOrientation(int orientation);
        void setPosition(Coordinates coord);
        // Acciones
        void startsSliding();
        void stopSliding();
        void isAlreadySliding();   //TODO mejorar nombres de slide!
        void startsKicking();
        void stopKicking();
        void isAlreadyKicking();

        ~Player();
};
#endif // PLAYER_H
