#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "util/Constants.h"
#include "util/logger.h"
#include "util/conf.h"
#include "model/Player.h"
#include "util/Coordinates.h"
#include "controller/actions/Action.h"
#include "controller/ActionsManager.h"
#include "view/PlayerSpriteManager.h"
#include "view/Texture.h"
#include "model/Team.h"
#include "view/PitchView.h"
#include "util/Colour.h"
#include "view/Camera.h"
#include "controller/GameController.h"
#include "model/Pitch.h"

#ifndef GAMEINITIALIZER_H
#define GAMEINITIALIZER_H

class GameInitializer {
    private:
<<<<<<< HEAD
        SDL_Window* window;
        SDL_Renderer* renderer;

        PitchView* pitchView;
        GameController* gameController;
        ActionsManager* actionsManager;
        Texture* localTeamSprites;
        Texture* awayTeamSprites;

=======
        GameController* gameController; // Liberado aca.
        ActionsManager* actionsManager; // Liberado aca.
>>>>>>> tp2-server-client

        Camera* camera; // Liberado en PitchView.
        Team* localTeam; // Liberado en Pitch.
        Pitch* pitch; // Liberado en game controller.
    public:
        GameInitializer(Conf* configuration);
        GameController* getGameController();
        ActionsManager* getActionsManager();
        Camera* getCamera();
        ~GameInitializer();
    private:
        void initializePitch(Conf* conf);
        void initializeLocalTeam(Conf* conf);
<<<<<<< HEAD
        void initializeBall();
        void initializeLocalTeamSprites(std::string path, Colour* shirt);
=======
>>>>>>> tp2-server-client
        void initializeGameController();
        void initializeActionsManager();
};
#endif // GAMEINITIALIZER_H
