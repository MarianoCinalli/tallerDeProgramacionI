#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "util/logger.h"

#ifndef CHANGEACTIVEPLAYER_H
#define CHANGEACTIVEPLAYER_H

class ChangeActivePlayer : public Action {
    public:
        ChangeActivePlayer();
        virtual void execute(Pitch* pitch);
        virtual bool valid(Pitch* pitch);
        virtual std::string getCommand();
        virtual std::vector<std::string> getParameters();
        ~ChangeActivePlayer();
};
#endif // CHANGEACTIVEPLAYER_H
