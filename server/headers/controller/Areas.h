#include "util/Rectangle.h"
#include "util/Constants.h"
#include "util/logger.h"
#include <map>

#ifndef AREAS_H
#define AREAS_H

class Areas {
    protected:
        std::map<int, Rectangle*> areasForPlayers;
    public:
        Areas();
        Rectangle* get(int playerNumber);
        virtual ~Areas();
    protected:
        std::string getPlayerWithAreasAsString();
        virtual void loadAreas();
        virtual void loadGoalkeeperArea();
        virtual void loadDefendersAreas();
        virtual void loadMidFildersAreas();
        virtual void loadAtackersAreas();
        void insertUnrestricted(int position);
};
#endif // AREAS_H
