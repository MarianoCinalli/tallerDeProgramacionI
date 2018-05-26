#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"
#include "util/logger.h"

#ifndef KICKINGACTION_H
#define KICKINGACTION_H

class KickingAction : public Action {
	private:
		int power;
    public:
        KickingAction(int power);
        virtual void execute(Player* player);
        virtual bool valid(Player* player);
        ~KickingAction();
};
#endif // KICKINGACTION_H
