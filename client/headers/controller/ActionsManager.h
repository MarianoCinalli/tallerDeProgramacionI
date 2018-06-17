#include "controller/actions/Action.h"
#include "model/Player.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "controller/actions/Accelerate.h"
#include "controller/actions/Stop.h"
#include "controller/actions/ChangeActivePlayer.h"
#include "controller/actions/SlidingAction.h"
#include "controller/actions/KickingAction.h"
#include "controller/actions/HighKickingAction.h"
#include "controller/actions/RunningFastAction.h"
#include "controller/actions/StopRunningFastAction.h"

#ifndef ACTIONSMANAGER_H
#define ACTIONSMANAGER_H

class ActionsManager {
    private:
      int kickTime;
    public:
        ActionsManager();
        bool keys[4] = {false};
        Action* getAction(SDL_Event event);
        bool shouldQuit(SDL_Event event);
        ~ActionsManager();
};
#endif // ACTIONSMANAGER_H
