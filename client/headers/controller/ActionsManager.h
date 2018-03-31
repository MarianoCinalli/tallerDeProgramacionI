#include "controller/actions/Action.h"
#include "view/Player.h"
#include "util/Constants.h"
#include "controller/actions/Accelerate.h"
#include "controller/actions/Decelerate.h"

#ifndef ACTIONSMANAGER_H
#define ACTIONSMANAGER_H

class ActionsManager {
    private:
    public:
        ActionsManager();
        Action* getAction(SDL_Event event);
        ~ActionsManager();
};
#endif // ACTIONSMANAGER_H


