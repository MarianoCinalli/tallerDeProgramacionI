#include "model/Player.h"
#include "util/Constants.h"
#include "util/logger.h"

#ifndef ACTION_H
#define ACTION_H

class Action {
    private:
    public:
        Action();
        virtual void execute(Player* player);
        virtual ~Action();
};
#endif // ACTION_H


