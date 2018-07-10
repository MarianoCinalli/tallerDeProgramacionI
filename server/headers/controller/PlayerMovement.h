#include "util/Constants.h"
#include "util/logger.h"
#include "util/Rectangle.h"
#include "util/Coordinates.h"
#include "util/Velocity.h"
#include "controller/PlayerAreas.h"

#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

class PlayerMovement {
    private:
        PlayerAreas* playerAreas;
        bool isLeftsideTeam;
        int formation;
    public:
        PlayerMovement();
        void setFormation(int formation);
        void setSide(bool isLeftsideTeam);
        bool isInsideArea(Coordinates* coordinates, int playerNumber);
        void cleanVelocity(Velocity* velocity, Coordinates* coordinates, int playerNumber);
        Coordinates* getCoordinatesToFollow(Coordinates* ballPosition, Coordinates* basePosition, int playerNumber, bool isAttacking);
        bool isDefender(int playerNumber);
        bool isAttacker(int playerNumber);
        std::string getDebugLines();
        ~PlayerMovement();
};
#endif // PLAYERMOVEMENT_H
