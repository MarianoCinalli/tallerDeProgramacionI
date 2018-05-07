#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"
#include "util/logger.h"

#ifndef RUNNINGFASTACTION_H
#define RUNNINGFASTACTION_H

class RunningFastAction : public Action {
	private:
    public:
        RunningFastAction();
        virtual void execute(Player* player);
        virtual bool valid(Player* player);
        virtual std::string getCommand();
        ~RunningFastAction();
};
#endif // RUNNINGFASTACTION_H