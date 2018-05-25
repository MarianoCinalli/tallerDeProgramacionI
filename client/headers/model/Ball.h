#include "util/Constants.h"
#include "util/Coordinates.h"
#include "util/logger.h"
#include "model/Player.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <yaml-cpp/yaml.h>

#ifndef BALL_H
#define BALL_H

class Ball {
    private:

        Coordinates* position;
        int still;

    public:
        Ball(Coordinates* position);
        int isStill();
        void parseYaml(YAML::Node node);
        Coordinates* getPosition();
        ~Ball();
};
#endif // BALL_H
