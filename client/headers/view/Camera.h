#include <list>
#include <SDL2/SDL.h>
#include "util/Coordinates.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "view/PlayerSpriteManager.h"


#ifndef CAMERA_H
#define CAMERA_H

class Camera {
    private:
        // La posicion es el punto de arriba a la izquierda del rectangulo.
        Coordinates* position;
        int width;
        int height;
    public:
        Camera(Coordinates* position, int width, int height);
        // transforma las coordenadas para que queden relativas a la camara.
        Coordinates* getRelativeCoordinates(Coordinates* coordinates);
        // devuelve los jugadores dentro de la camara.
        std::list<PlayerSpriteManager*> getPlayersInside(std::list<PlayerSpriteManager*>& views);
        // devuelve el SDL_Rect para dibujar la parte de la cancha.
        SDL_Rect getRectToDraw();
        ~Camera();
};
#endif // CAMERA_H


