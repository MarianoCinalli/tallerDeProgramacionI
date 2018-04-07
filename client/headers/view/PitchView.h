#include <list>
#include "view/PlayerSpriteManager.h"

/*
VistaCancha:
        - Tiene las vistas de los jugadores.
        - Tiene la textura de la cancha. (falta)
        - render() dibuja todo.
        Primero la cancha y despues a cada player view,
        les transforma las coordenadas para que queden el la
        ventana (falta el metodo get coordinates y agregarle
        una referencia al Player model dentro de PlayerSpriteManager)
        y despues le hace el render() a todas los PlayerSpriteManager.
*/

#ifndef PITCHVIEW_H
#define PITCHVIEW_H

class PitchView {
    private:
        std::list<PlayerSpriteManager*> playerViews;
    public:
        PitchView();
        void addPlayerViews();
        void render();
        ~PitchView();
};
#endif // PITCHVIEW_H


