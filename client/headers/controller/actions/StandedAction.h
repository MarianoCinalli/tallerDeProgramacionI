#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"
#include "util/logger.h"

#ifndef STANDEDACTION_H
#define STANDEDACTION_H

class StandedAction : public Action {
    private:
        int action;
    public:
        StandedAction(int action);
        virtual void execute(Player* player);
        ~StandedAction();
};
#endif // STANDEDACTION_H