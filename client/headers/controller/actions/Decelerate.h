#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "view/Player.h"

#ifndef DECELERATE_H
#define DECELERATE_H

class Decelerate : public Action {
    private:
        int direction;
    public:
        Decelerate(int direction);
        void execute(Player* player);
        ~Decelerate();
};
#endif // DECELERATE_H


