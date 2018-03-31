#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"

#ifndef ACCELERATE_H
#define ACCELERATE_H

class Accelerate : public Action {
    private:
        int direction;
    public:
        Accelerate(int direction);
        void execute(Player* player);
        ~Accelerate();
};
#endif // ACCELERATE_H


