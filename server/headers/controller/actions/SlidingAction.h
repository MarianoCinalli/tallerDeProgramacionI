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
        virtual bool valid(Player* player);
				virtual void execute(Pitch* pitch, std::string user);
				virtual bool valid(Pitch* pitch);
        ~SlidingAction();
};
#endif // SLIDINGACTION_H
