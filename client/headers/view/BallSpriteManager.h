#include "util/Coordinates.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "view/Texture.h"
#include "model/Ball.h"
#include <SDL2/SDL.h>

#ifndef BALLSPRITEMANAGER_H
#define BALLSPRITEMANAGER_H

class BallSpriteManager {
    private:
        Ball* ball;
        Texture* spriteSheet;
        SDL_Rect sprite;
        int rollingCount;
    public:
        BallSpriteManager(Texture* spriteSheet, Ball* ball);
        void render(SDL_Renderer* screen, Coordinates* coordinates);
        Coordinates* getBallCoordinates();
        ~BallSpriteManager();
    private:
        SDL_Rect getPositionOnScreen(SDL_Rect sprite, Coordinates* coordinates);

        void setMovingBallSprite();
};
#endif // PLAYERSPRITEMANAGER_H
