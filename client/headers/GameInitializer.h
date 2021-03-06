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
#include "model/Ball.h"

#ifndef GAMEINITIALIZER_H
#define GAMEINITIALIZER_H

class GameInitializer {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        PitchView* pitchView;
        GameController* gameController;
        ActionsManager* actionsManager;
        Texture* localTeamSprites;
        Texture* awayTeamSprites;

        Camera* camera; // Liberado en PitchView.
        Team* localTeam; // Liberado en Pitch.
        Team* awayTeam; // Liberado en Pitch.
        Pitch* pitch; // Liberado en game controller.

        //Clock* clock;
        //Minimap* minimap;
        //Score* score;
    public:
        GameInitializer(Conf* configuration);
        GameController* getGameController();
        ActionsManager* getActionsManager();
        Camera* getCamera();
        SDL_Renderer* getRenderer();
        PitchView* getPitchView();
        Team* getTeam(int teamNumber);
        ~GameInitializer();
    private:
        void initializePitch(Conf* conf);
        void initializePitchView(Conf* conf);
        void initializeTeam(Conf* conf, int teamNumber);
        void initializeTeamSprites(std::string path, Colour* shirt, int teamNumber);
        void initializeGameController();
        void initializeBall();
        void initializeActionsManager();
        void initializeCommonSdlResources();
        void cleanCommonSdlResources();
};
#endif // GAMEINITIALIZER_H
