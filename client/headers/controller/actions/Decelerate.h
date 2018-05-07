#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"
#include "util/logger.h"

#ifndef DECELERATE_H
#define DECELERATE_H

class Decelerate : public Action {
    private:
        int direction;
    public:
        Decelerate(int direction);
        virtual void execute(Player* player);
        virtual bool valid(Player* player);
        virtual std::string getCommand();
        ~Decelerate();
};
#endif // DECELERATE_H
