#include "util/Constants.h"
#include "view/Player.h"

#ifndef DECELERATE_H
#define DECELERATE_H

class Decelerate : Action {
    private:
    public:
        Decelerate();
        void excecute(Player* player);
        ~Decelerate();
};
#endif // DECELERATE_H


