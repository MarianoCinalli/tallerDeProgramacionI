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
        std::string userName;
        Player(int orientation, Coordinates* position, int teamNumber);

        // Getters
        Coordinates* getPosition();
        int getOrientation();

        bool isSliding();
        bool isKicking();
        bool getIsSelected();
        bool isRunningFast();
        bool isStill();

        // Modifiers
        void parseYaml(YAML::Node node);

        ~Player();
};
#endif // PLAYER_H
