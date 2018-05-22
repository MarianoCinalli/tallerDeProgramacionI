#include "util/Constants.h"
#include "util/Coordinates.h"
#include "util/Velocity.h"
#include "util/logger.h"
#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#ifndef PLAYER_H
#define PLAYER_H

class Player {
    private:
        int orientation;
        int maxVelocity;
        int team;
        bool isSelected;
        bool isReturning;
        Velocity* velocity;
        Coordinates* position;
        Coordinates* basePosition;
        bool sliding;
        bool wasSliding;
        bool kicking;
        bool wasKicking;
        bool canMove;
        bool runningFast;
        bool dominatesTheBall;
        bool still;

    public:
        Player(int orientation, Coordinates* position, int teamNumber);

        // Getters
        Coordinates* getPosition();
        Velocity* getVelocity();
        int getCurrentSpeed();
        int getOrientation();
        bool wasSlidingYet();
        bool wasKickingYet();
        bool isSliding();
        bool isKicking();
        bool getIsSelected();
        bool isRunningDiagonaly();
        bool isRunningFast();

        // Modifiers
        void parseYaml(YAML::Node node);
        // Cambia la velocidad a su maximo, por ahora.
        void accelerate(int direction);
        void decelerate(int direction);
        void stop();
        void stopRunningInDirection(int direction);
        void toggleIsSelected();
        void copyStats(Player* copyTo);
        void setTrayectory(Velocity* trayectory);
        // Cambia la posicion con respecto a su velocidad.
        void updatePosition();
        void setOrientation(int orientation);
        void setPosition(Coordinates coord);
        void setBasePosition(Coordinates coord);
        void returnToBasePosition();
        // Acciones
        void startsRunningFast();
        void stopsRunningFast();
        void startsSliding();
        void stopSliding();
        void isAlreadySliding();   //TODO mejorar nombres de slide!
        void startsKicking();
        void stopKicking();
        void isAlreadyKicking();
        bool isStill();

        ~Player();
};
#endif // PLAYER_H
