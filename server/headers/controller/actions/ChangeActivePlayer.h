#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "util/logger.h"

#ifndef CHANGEACTIVEPLAYER_H
#define CHANGEACTIVEPLAYER_H

class ChangeActivePlayer : public Action {
    public:
        ChangeActivePlayer();
        virtual void execute(Pitch* pitch, std::string user);
        virtual bool valid(Pitch* pitch);
        ~ChangeActivePlayer();
};
#endif // CHANGEACTIVEPLAYER_H
