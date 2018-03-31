#include "util/Constants.h"
#include "view/Player.h"

#ifndef ACCELERATE_H
#define ACCELERATE_H

class Accelerate : Action {
    private:
        int direction;
    public:
        Accelerate();
        void excecute(Player* player);
        ~Accelerate();
};
#endif // ACCELERATE_H


