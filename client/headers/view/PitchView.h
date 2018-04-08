#include <list>
#include "util/Coordinates.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "view/PlayerSpriteManager.h"

/*
VistaCancha:
    - Tiene las vistas de los jugadores.
    - Tiene la textura de la cancha. (falta)
    - Tiene una clase camara (falta):
        - Un metodo devuelve el SDL_Rect para dibujar la parte de
        la cancha.
        - Un metodo devuelve los jugadores dentro de la camara.
        - Otro metodo transforma las coordenadas para que queden
        relativas a la camara.
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
        std::list<PlayerSpriteManager*> playerViews;
    public:
        PitchView();
        void addPlayerView(PlayerSpriteManager* playerView);
        void render();
        ~PitchView();
};
#endif // PITCHVIEW_H


