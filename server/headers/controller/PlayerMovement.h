#include "util/Constants.h"
#include "util/logger.h"
#include "util/Rectangle.h"
#include "controller/PlayerAreas.h"

#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

class PlayerMovement {
    private:
        PlayerAreas* playerAreas;
    public:
        PlayerMovement();
        void setFormation(int formation);
        bool canMoveTo(int x, int y, int playerNumber, bool isSelected);
        bool isInsideArea(int x, int y, int playerNumber);
        std::string getDebugLines();
        ~PlayerMovement();
};
#endif // PLAYERMOVEMENT_H
