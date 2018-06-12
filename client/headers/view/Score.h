#include <list>
#include <SDL2/SDL.h>
#include "util/Coordinates.h"
#include "util/Constants.h"
#include "util/logger.h"
#include <yaml-cpp/yaml.h>
#include <SDL2/SDL_ttf.h>

#ifndef SCORE_H
#define SCORE_H

class Score {
    private:
        TTF_Font* gFont;
    public:
        int local;
        int visitante;
        Score();
        // parsea YAML
        void parseYaml(YAML::Node node);
        void render(SDL_Renderer* screen);
        ~Score();
};
#endif // SCORE_H
