#include "controller/Action.h"
#include "util/Constants.h"

#ifndef ACTIONSMANAGER_H
#define ACTIONSMANAGER_H

class ActionsManager {
    private:
    public:
        ActionsManager();
        Action* getAction(SDL_Event* event);
        ~ActionsManager();
};
#endif // ACTIONSMANAGER_H


