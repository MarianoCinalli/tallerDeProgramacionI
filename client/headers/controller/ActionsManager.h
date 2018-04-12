#include "controller/actions/Action.h"
#include "model/Player.h"
#include "util/Constants.h"
#include "controller/actions/Accelerate.h"
#include "controller/actions/Decelerate.h"
#include "util/logger.h"
#include "controller/actions/SlidingAction.h"
#include "controller/actions/KickingAction.h"

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


