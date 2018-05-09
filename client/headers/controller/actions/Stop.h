#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"
#include "util/logger.h"

#ifndef STOP_H
#define STOP_H

class Stop : public Action {
    private:
        int direction;
    public:
        Stop();
        virtual void execute(Player* player);
        virtual bool valid(Player* player);
        virtual std::string getCommand();
        virtual std::vector<std::string> getParameters();
        ~Stop();
};
#endif // STOP_H
