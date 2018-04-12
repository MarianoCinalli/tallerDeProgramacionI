#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"
#include "util/logger.h"

#ifndef SLIDINGACTION_H
#define SLIDINGACTION_H

class SlidingAction : public Action {
	private:
    public:
        SlidingAction();
        virtual void execute(Player* player);
        ~SlidingAction();
};
#endif // SLIDINGACTION_H