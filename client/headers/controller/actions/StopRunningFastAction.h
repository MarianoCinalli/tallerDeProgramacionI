#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"
#include "util/logger.h"

#ifndef STOPRUNNINGFASTACTION_H
#define STOPRUNNINGFASTACTION_H

class StopRunningFastAction : public Action {
	private:
    public:
        StopRunningFastAction();
        virtual void execute(Player* player);
        virtual bool valid(Player* player);
        virtual std::string getCommand();
        virtual std::string getParameters();
        ~StopRunningFastAction();
};
#endif // STOPRUNNINGFASTACTION_H