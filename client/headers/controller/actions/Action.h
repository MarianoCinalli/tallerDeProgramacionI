#include "model/Player.h"

#ifndef ACTION_H
#define ACTION_H

class Action {
    private:
    public:
        Action();
        void execute(Player* player);
        ~Action();
};
#endif // ACTION_H


