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
        int margin_size;
        // La camara tiene sus margenes
        SDL_Rect margin;
    public:
        Camera(Coordinates* position, int width, int height, int margin);
        // transforma las coordenadas para que queden relativas a la camara.
        Coordinates* getRelativeCoordinates(Coordinates* coordinates);
        // calcular la nueva posicion de la camara basado en el jugador activo
        void calculateNewPostion(Coordinates* playerPosition, int playerSpeed);
        // devuelve los jugadores dentro del margen.
        std::list<Player*> getPlayersInsideMargin(std::list<Player*> players, int outerMargin);
        // devuelve los jugadores dentro de la camara.
        bool isInsideMargin(Player* p, SDL_Rect* margin, int outerMargin);
        std::list<PlayerSpriteManager*> getPlayersInside(std::list<PlayerSpriteManager*>& views);
        // devuelve el SDL_Rect para dibujar la parte de la cancha.
        SDL_Rect getRectToDraw();
        ~Camera();
};
#endif // CAMERA_H
