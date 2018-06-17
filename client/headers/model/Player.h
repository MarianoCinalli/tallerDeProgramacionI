#include "util/Constants.h"
#include "util/Coordinates.h"
#include "util/logger.h"
#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#ifndef PLAYER_H
#define PLAYER_H

class Player {
    private:
        int orientation;
        int team;
        bool isSelected;
        bool isReturning;
        Coordinates* position;
        bool sliding;
        bool kicking;
        bool runningFast;
        bool still;


    public:
        std::string userName;
        Player(int orientation, Coordinates* position, int teamNumber);

        // Getters
        Coordinates* getPosition();
        int getOrientation();
        int getTeam();

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
