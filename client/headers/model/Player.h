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
        bool isSelected;
        Velocity* velocity;
        Coordinates* position;
        bool sliding;
        bool wasSliding;
        bool kicking;
        bool wasKicking;
        bool canMove;
        bool runningFast;

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
        bool getIsSelected();
        bool isRunningDiagonaly();
        bool isRunningFast();

        // Modifiers
        // Cambia la velocidad a su maximo, por ahora.
        void accelerate(int direction);
        void decelerate(int direction);
        void stop(int direction);
        void stopRunningInDirection(int direction);
        void toggleIsSelected();
        // Cambia la posicion con respecto a su velocidad.
        void updatePosition();
        void setOrientation(int orientation);
        void setPosition(Coordinates coord);
        // Acciones
        void startsRunningFast();
        void stopsRunningFast();
        void startsSliding();
        void stopSliding();
        void isAlreadySliding();   //TODO mejorar nombres de slide!
        void startsKicking();
        void stopKicking();
        void isAlreadyKicking();

        ~Player();
};
#endif // PLAYER_H
