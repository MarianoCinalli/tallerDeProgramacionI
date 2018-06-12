#include <list>
#include <SDL2/SDL.h>
#include "util/Coordinates.h"
#include "util/Constants.h"
#include "util/logger.h"
#include <yaml-cpp/yaml.h>


#ifndef SCORE_H
#define SCORE_H

class Score {
    private:

    public:
        Score();
        // parsea YAML
        void parseYaml(YAML::Node node);
        ~Score();
};
#endif // SCORE_H
