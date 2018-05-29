#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"
#include "util/logger.h"

#ifndef HIGHKICKINGACTION_H
#define HIGHKICKINGACTION_H

class HighKickingAction : public Action {
	private:
		int power;
    public:
        HighKickingAction(int power);
        virtual void execute(Player* player);
        virtual bool valid(Player* player);
        ~HighKickingAction();
};
#endif // HIGHKICKINGACTION_H