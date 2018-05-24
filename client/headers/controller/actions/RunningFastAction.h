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
        virtual std::string getCommand();
        virtual std::string getParameters();
        ~RunningFastAction();
};
#endif // RUNNINGFASTACTION_H
