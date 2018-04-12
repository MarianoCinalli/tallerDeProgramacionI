#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"
#include "util/logger.h"

#ifndef KICKINGACTION_H
#define KICKINGACTION_H

class KickingAction : public Action {
	private:
    public:
        KickingAction();
        virtual void execute(Player* player);
        ~KickingAction();
};
#endif // KICKINGACTION_H