#include "util/Constants.h"
#include "util/Coordinates.h"
#include "util/Velocity.h"
#include "util/logger.h"
#include "model/Player.h"
#include <SDL2/SDL.h>
#include <math.h>

#ifndef BALL_H
#define BALL_H

class Ball {
    private:
        Velocity* velocity;
        int maxVelocity;
        Coordinates* position;
        float passPower;
        float maxSpeed;
        float decelerate;
        float initialPassPower;
        int height;
        int heightLevel;
        int orientation;
        bool dominated;
        bool isInAPass;
        bool isInAHighPass;
        bool interceptable;
        int passDirection;
        Coordinates* startingPassPosition;
        int decelerateDistance;
        int timeIntercept;
        int decelerateLevel;
        Player* player;

    public:
        Ball(Coordinates* position, float speed, float decel);

        Coordinates* getPosition();
        void setPosition(Coordinates* pos);
        Velocity* getVelocity();
        int getHeight();
        Player* getPlayer();
        void setPlayer(Player* player);
        bool isDominated();
        bool isInterceptable();
        void restart(int position);
        void isIntercepted(Player* player);
        void isPassed(Velocity* velocity, float passPower, bool highPass);
        void updatePosition();

        void calculateDominatedPosition();
        void progressiveDecelerate(float passDistance);
        void stopRolling();
        float calculatePassPower(float passPower, int timePassing);
        void calculateHeight();
        bool doesHisTeamHaveTheBall(Player* player);

        std::string getAsYaml();

        ~Ball();
};
#endif // BALL_H
