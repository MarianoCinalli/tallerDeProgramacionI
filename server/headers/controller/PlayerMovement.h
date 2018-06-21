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
    public:
        PlayerMovement();
        void setFormation(int formation);
        bool isInsideArea(Coordinates* coordinates, int playerNumber);
        void cleanVelocity(Velocity* velocity, Coordinates* coordinates, int playerNumber);
        std::string getDebugLines();
        ~PlayerMovement();
};
#endif // PLAYERMOVEMENT_H
