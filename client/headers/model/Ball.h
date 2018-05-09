#include "util/Constants.h"
#include "util/Coordinates.h"
#include "util/Velocity.h"
#include "util/logger.h"
#include <SDL2/SDL.h>

#ifndef BALL_H
#define BALL_H

class Ball {
    private:
        Velocity* velocity;
        int maxVelocity;
        Coordinates* position;
        int passPower;
        int height;
        int orientation;
        bool isDominated;
        Velocity* velocity;
        bool isInAPass;
        int passDirection;
        int decelerateLevel;
        Player* player;

    public:
        Ball(Coordinates* position, Player* player);

        Coordinates* getPosition();
        Velocity* getVelocity();
        bool isDominated();
        
        void isIntercepted(Player* player);
        void isPassed(int direction, int passPower);
        void updatePosition();

        Coordinates* calculateDominatedPosition();
        void progressiveDecelerate(float passDistance);
        void stopRolling();

        ~Ball();
};
#endif // BALL_H
