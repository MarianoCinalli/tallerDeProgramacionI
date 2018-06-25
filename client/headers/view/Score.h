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
        SDL_Renderer* renderer;
        SDL_Rect scoreViewport;
    public:
        int local;
        int visitante;
        Score();
        // parsea YAML
        void parseYaml(YAML::Node node);
        void render(SDL_Renderer* screen);
        void gol();
        void golLocal();
        void golVisitante();
        ~Score();
};
#endif // SCORE_H
