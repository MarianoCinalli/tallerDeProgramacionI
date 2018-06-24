#include "util/Constants.h"
#include "util/logger.h"
#include "util/Rectangle.h"
#include "controller/Areas.h"
#include "controller/Areas312.h"
#include "controller/Areas321.h"
#include "controller/Areas33.h"

#ifndef PLAYERAREAS_H
#define PLAYERAREAS_H

class PlayerAreas {
    private:
        Areas* areas;
    public:
        PlayerAreas();
        PlayerAreas(int formation);
        Rectangle* getForPlayer(int playerNumber, bool isLeftsideTeam);
        std::string getDebugLines();
        ~PlayerAreas();
};
#endif // PLAYERAREAS_H
