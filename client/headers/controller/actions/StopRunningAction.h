#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"
#include "util/logger.h"

#ifndef STOPRUNNINGACTION_H
#define STOPRUNNINGACTION_H

class StopRunningAction : public Action {
    private:
        int direction;
    public:
        StopRunningAction(int direction);
        virtual void execute(Player* player);
        virtual bool valid(Player* player);
        virtual std::string getCommand();
        ~StopRunningAction();
};
#endif // STOPRUNNINGACTION_H