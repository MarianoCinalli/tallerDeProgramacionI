#include <list>
#include "util/Coordinates.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "view/PlayerSpriteManager.h"
#include "view/BallSpriteManager.h"
#include "view/Texture.h"
#include "view/Camera.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>



/*
VistaCancha:
    - Tiene las vistas de los jugadores.
    - Tiene la textura de la cancha. (falta)
    - Tiene la camara.
        La camara la va a actualizar algun controlador para que se
        mueva conforme se muevan los jugadores en los bordes.
        Por eso la va a recibir en el constructor.
    - render()
        Aca se dibuja.
        Primero dibuja la parte de la cancha que la camara indica.
        Despues obtiene una lista de vistas de jugadores dentro de la
        camara, se recorre esa lista de jugadores y para cada jugador
        se llama a un metodo de camara que va a transformar las
        coordenadas para que queden relativas.
*/

#ifndef PITCHVIEW_H
#define PITCHVIEW_H

class PitchView {
    private:
        Texture* pitch;
        Texture* pitchMini;
        Camera* camera;
        std::list<PlayerSpriteManager*> playerViews;
        BallSpriteManager* ballView;
        int messageTime;
    public:
        PitchView(Texture* pitch, Texture* pitchMini, Camera* camera);
        void addPlayerView(PlayerSpriteManager* playerView);
        void addBallView(BallSpriteManager* ballView);
        void renderMinimap(SDL_Renderer* screen);
        void renderMessage(SDL_Renderer* screen, std::string msg);

        void renderCountdown(SDL_Renderer* screen, int countdown);

        void render(SDL_Renderer* screen);
        ~PitchView();
    private:
        void renderPitch(SDL_Renderer* screen);
};
#endif // PITCHVIEW_H
