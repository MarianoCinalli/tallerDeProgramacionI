#include "util/Coordinates.h"
#include "util/Velocity.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "model/Ball.h"
#include <SDL2/SDL.h>

#ifndef BALLSPRITEMANAGER_H
#define BALLSPRITEMANAGER_H

class BallSpriteManager {
    private:
        Ball* ball;
        SDL_Rect sprite;

    public:
        BallSpriteManager(Ball* ball);
        void render(SDL_Renderer* screen, Coordinates* coordinates);
        Coordinates* getBallCoordinates();
        ~BallSpriteManager();
    private:
        SDL_Rect getPositionOnScreen(SDL_Rect sprite, Coordinates* coordinates);

        void setMovingBallSprite(Velocity* velocity);
};
#endif // PLAYERSPRITEMANAGER_H
