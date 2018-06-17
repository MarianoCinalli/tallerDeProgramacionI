#include "controller/PlayerAreas.h"
#include "util/Rectangle.h"

#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

class PlayerMovement {
    private:
        PlayerAreas* playerAreas;
    public:
        PlayerMovement();
        void setFormation(int formation);
        bool canMoveTo(int x, int y, int playerNumber, bool isSelected);
        ~PlayerMovement();
};
#endif // PLAYERMOVEMENT_H
