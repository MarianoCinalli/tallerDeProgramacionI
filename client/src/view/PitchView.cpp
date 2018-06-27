#include "view/PitchView.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

extern Mix_Chunk *gWhistleSound;

PitchView::PitchView(Texture* pitch, Texture* pitchMini, Camera* camera) {
        log("PitchView: Creando vista de la cancha...", LOG_INFO);
        this->pitch = pitch;
        this->pitchMini = pitchMini;
        this->camera = camera;
        this->messageTime = -1;
        this->font = TTF_OpenFont("lazy.ttf", 30);
        if (this->font == NULL) {
                log("openLoginFormacion: Error al cargar la fuente! SDL_ttf Error: ", TTF_GetError(), LOG_INFO);
        }
        log("PitchView: Vista de la cancha creada...", LOG_INFO);
}

void PitchView::addPlayerView(PlayerSpriteManager* playerView) {
        this->playerViews.push_back(playerView);
}

void PitchView::addBallView(BallSpriteManager* ballView) {
        this->ballView = ballView;
}

void PitchView::renderMinimap(SDL_Renderer* screen) {
        // Este es el viewPort del Minimap
        SDL_Rect minimapViewport;
        minimapViewport.x = 300; //SCREEN_WIDTH / 3;
        minimapViewport.y = 0;
        minimapViewport.w = 200; //SCREEN_WIDTH / 3;
        minimapViewport.h = 100;
        SDL_RenderSetViewport( screen, &minimapViewport ); //Render texture to screen
        // Dibujo los bordes del minimapViewport
        SDL_Rect outlineRect = { 0, 0, minimapViewport.w, minimapViewport.h };
        SDL_SetRenderDrawColor( screen, 0xFF, 0xFF, 0xFF, 0xFF ); //BLANCO
        SDL_RenderFillRect(screen, &outlineRect);

        // Dibujar imagen de la cancha
        SDL_RenderCopy( screen, this->pitchMini->getSpriteSheetTexture(), NULL, NULL );

        SDL_SetRenderDrawColor( screen, 0x00, 0x00, 0x00, 0xFF ); //NEGRO
        SDL_RenderDrawRect( screen, &outlineRect );

        int team = 0;

        // TODO pasar a Constants
        double MINIMAP_SCALE_X = 0.13;
        double MINIMAP_SCALE_Y = 0.105;

        // Dibujar la camara
        Coordinates coordinatesCam = this->camera->getPosition();
        SDL_Rect cameraRect;
        cameraRect.x = coordinatesCam.getX() * MINIMAP_SCALE_X;
        cameraRect.y = coordinatesCam.getY() * MINIMAP_SCALE_Y;
        cameraRect.w = 76;//hacer proporcion
        cameraRect.h = 46;//hacer proporcion
        SDL_SetRenderDrawColor( screen, 0xFF, 0x00, 0x00, 0xFF ); //ROJO
        SDL_RenderDrawRect( screen, &cameraRect );

        // Bibujar la pelota
        Coordinates* coordinatesBall = this->ballView->getBallCoordinates();
        SDL_Rect ballRect = { coordinatesBall->getX() * MINIMAP_SCALE_X, coordinatesBall->getY() * MINIMAP_SCALE_Y, 3, 3 };
        SDL_SetRenderDrawColor( screen, 0xFF, 0xFF, 0xFF, 0xFF ); //BLANCO
        SDL_RenderFillRect( screen, &ballRect );

        // Obtener la posicion de todos los jugadores
        std::list<PlayerSpriteManager*> views = this->camera->getPlayersInside(this->playerViews);
        // Dibujar cada uno.
        std::list<PlayerSpriteManager*>::iterator viewIter;
        for (viewIter = views.begin(); viewIter != views.end(); viewIter++) {
                // Coordenadas absolutas
                Coordinates* coordinates = (*viewIter)->getPlayerCoordinates();
                SDL_Rect playerRect = { coordinates->getX() * MINIMAP_SCALE_X, coordinates->getY() * MINIMAP_SCALE_Y, 3, 3 };
                team = (*viewIter)->getPlayerTeam();
                if (team==0) {
                        // equipo Rojo
                        SDL_SetRenderDrawColor( screen, 0x00, 0xCC, 0xCC, 0xFF ); //CELESTE
                } else {
                        SDL_SetRenderDrawColor( screen, 0xFF, 0xFF, 0x00, 0xFF ); //AMARILLO
                }
                SDL_RenderFillRect( screen, &playerRect );
        }
}

void PitchView::renderCountdown(SDL_Renderer* screen, int countdown){
        SDL_Rect cancha;
        cancha.x = 0;
        cancha.y = CAMERA_OFFSET;
        cancha.w = SCREEN_WIDTH;
        cancha.h = SCREEN_HEIGHT - CAMERA_OFFSET;
        SDL_RenderSetViewport( screen, &cancha ); //Render texture to screen
        std::string message = "EL juego empieza en ";
        message += std::to_string(5-countdown);
        SDL_Color SDL_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };
        Texture countdownTexture;
        if (!countdownTexture.loadFromRenderedText(message, screen, SDL_WHITE, this->font)) {
                log("No se pudo mostrar el texto", LOG_ERROR);
        }else{
                SDL_Rect renderQuad1 = { (SCREEN_WIDTH - countdownTexture.getWidth()) / 2, 150, countdownTexture.getWidth(), countdownTexture.getHeight() };
                SDL_RenderCopyEx(screen, countdownTexture.getSpriteSheetTexture(), NULL, &renderQuad1, 0.0, NULL, SDL_FLIP_NONE);
                // SDL_RenderPresent(screen);
                if (countdown >= 5) {
                        Mix_PlayChannel( -1, gWhistleSound, 0 );
                }
        }

}

void PitchView::renderMessage(SDL_Renderer* screen, std::string message){
        SDL_Rect cancha;
        cancha.x = 0;
        cancha.y = CAMERA_OFFSET;
        cancha.w = SCREEN_WIDTH;
        cancha.h = SCREEN_HEIGHT - CAMERA_OFFSET;
        SDL_RenderSetViewport( screen, &cancha ); //Render texture to screen
        SDL_Color SDL_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };
        Texture messageTexture;
        if (!messageTexture.loadFromRenderedText(message, screen, SDL_WHITE, this->font)) {
                log("No se pudo mostrar el texto", LOG_ERROR);
        }
        else{
                SDL_Rect renderQuad1 = { (SCREEN_WIDTH - messageTexture.getWidth()) / 2, 150, messageTexture.getWidth(), messageTexture.getHeight() };
                SDL_RenderCopyEx(screen, messageTexture.getSpriteSheetTexture(), NULL, &renderQuad1, 0.0, NULL, SDL_FLIP_NONE);
                // SDL_RenderPresent(screen);
        }
}


void PitchView::renderDebug(SDL_Renderer* screen, std::string message){
        SDL_Rect cancha;
        cancha.x = 0;
        cancha.y = CAMERA_OFFSET;
        cancha.w = SCREEN_WIDTH;
        cancha.h = SCREEN_HEIGHT - CAMERA_OFFSET;
        SDL_RenderSetViewport( screen, &cancha ); //Render texture to screen
        SDL_Color SDL_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };
        Texture debugTexture;
        if (!debugTexture.loadFromRenderedText(message, screen, SDL_WHITE, this->font)) {
                log("No se pudo mostrar el texto", LOG_ERROR);
        }
        else{
                SDL_Rect renderQuad1 = { 0, 50, debugTexture.getWidth(), debugTexture.getHeight() };
                SDL_RenderCopyEx(screen, debugTexture.getSpriteSheetTexture(), NULL, &renderQuad1, 0.0, NULL, SDL_FLIP_NONE);
                // SDL_RenderPresent(screen);
        }
}


void PitchView::render(SDL_Renderer* screen) {
        //log("PitchView: Comienza la renderizacion...", LOG_SPAM);
        // Dibujar cancha vista por camara.
        SDL_Rect cancha;
        cancha.x = 0;
        cancha.y = CAMERA_OFFSET;
        cancha.w = SCREEN_WIDTH;
        cancha.h = SCREEN_HEIGHT - CAMERA_OFFSET;
        SDL_RenderSetViewport( screen, &cancha ); //Render texture to screen

        this->renderPitch(screen);
        // Dibuja la pelota, asi los jugadores quedan arriba
        Coordinates* coordinates = this->camera->getRelativeCoordinates(this->ballView->getBallCoordinates());

        this->ballView->render(screen, coordinates);
        delete(coordinates);
        // Obtener los jugadores vistos por la camara
        std::list<PlayerSpriteManager*> views = this->camera->getPlayersInside(this->playerViews);
        // Dibujar cada uno.
        std::list<PlayerSpriteManager*>::iterator viewIter;
        for (viewIter = views.begin(); viewIter != views.end(); viewIter++) {
                // Relativizar las coordenadas a la camara.
                Coordinates* coordinates = this->camera->getRelativeCoordinates(
                        (*viewIter)->getPlayerCoordinates()
                        );

                (*viewIter)->render(screen, coordinates);
                delete(coordinates);
        }
        //log("PitchView: Fin de renderizacion.", LOG_SPAM);
        // SDL_RenderPresent(screen);
}

PitchView::~PitchView() {
        log("PitchView: Liberando memoria...", LOG_INFO);
        // Borro la imagen de la cancha.
        log("PitchView: Borrando cancha...", LOG_INFO);
        delete(this->pitch);
        log("PitchView: Borrando camara...", LOG_INFO);
        delete(this->camera);
        // Libero todos los player views.
        log("PitchView: Borrando vista de jugadores...", LOG_INFO);
        std::list<PlayerSpriteManager*>::iterator it;
        for (it = this->playerViews.begin(); it != this->playerViews.end(); it++) {
                delete(*it);
        }
        TTF_CloseFont( this->font );
        this->font = NULL;
        // Como la lista sigue llena, de punteros borrados, la vacio.
        this->playerViews.clear();
        log("PitchView: Memoria liberada.", LOG_INFO);
}

void PitchView::renderPitch(SDL_Renderer* screen) {
        SDL_Rect que = this->camera->getRectToDraw();
        SDL_RenderCopy(
                screen,
                this->pitch->getSpriteSheetTexture(),
                &que,
                NULL
                );
}
