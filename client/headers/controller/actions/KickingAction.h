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
        virtual std::string getCommand();
        virtual std::string getParameters();
        ~KickingAction();
};
#endif // KICKINGACTION_H
