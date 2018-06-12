#include <list>
#include <SDL2/SDL.h>
#include "util/Coordinates.h"
#include "util/Constants.h"
#include "util/logger.h"
#include <yaml-cpp/yaml.h>


#ifndef CLOCK_H
#define CLOCK_H

class Clock {
    private:

    public:
        std::string value;
        Clock();
        // parsea YAML
        void parseYaml(YAML::Node node);
        void render(SDL_Renderer* screen);
        ~Clock();
};
#endif // CLOCK_H
