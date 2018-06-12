#include "view/PitchView.h"

PitchView::PitchView(Texture* pitch, Camera* camera) {
    log("PitchView: Creando vista de la cancha...", LOG_INFO);
    this->pitch = pitch;
    this->camera = camera;
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
    minimapViewport.x = SCREEN_WIDTH / 3;
    minimapViewport.y = 0;
    minimapViewport.w = SCREEN_WIDTH / 3;
    minimapViewport.h = 100;
    SDL_RenderSetViewport( screen, &minimapViewport ); //Render texture to screen
    // Dibujo los bordes del minimapViewport
    SDL_Rect outlineRect = { 0, 0, minimapViewport.w, minimapViewport.h };
    SDL_SetRenderDrawColor( screen, 0x00, 0xFF, 0x00, 0xFF ); //VERDE
    SDL_RenderDrawRect( screen, &outlineRect );

    // TODO pasar a Constants
    double MINIMAP_SCALE = 0.2;

    // Dibujar la camara
    Coordinates coordinatesCam = this->camera->getPosition();
    SDL_Rect cameraRect;
    cameraRect.x = coordinatesCam.getX() * MINIMAP_SCALE;
    cameraRect.y = coordinatesCam.getY() * MINIMAP_SCALE;
    cameraRect.w = 40;//hacer proporcion
    cameraRect.h = 25;//hacer proporcion
    SDL_SetRenderDrawColor( screen, 0xFF, 0x00, 0x00, 0xFF ); //ROJO
    SDL_RenderDrawRect( screen, &cameraRect );

    // Bibujar la pelota
    // Coordinates* coordinatesBall = this->ballView->getBallCoordinates();
    // SDL_Rect ballRect = { coordinatesBall->getX(), coordinatesBall->getY(), 3, 3 };
    // SDL_RenderFillRect( screen, &ballRect );
    // delete(coordinatesBall);

    // Obtener la posicion de todos los jugadores
    std::list<PlayerSpriteManager*> views = this->camera->getPlayersInside(this->playerViews);
    // Dibujar cada uno.
    std::list<PlayerSpriteManager*>::iterator viewIter;
    for (viewIter = views.begin(); viewIter != views.end(); viewIter++) {
        // Relativizar las coordenadas a la camara.
        Coordinates* coordinates = this->camera->getRelativeCoordinates(
            (*viewIter)->getPlayerCoordinates()
        );
        SDL_Rect playerRect = { coordinates->getX() * MINIMAP_SCALE, coordinates->getY() * MINIMAP_SCALE, 3, 3 };
        SDL_SetRenderDrawColor( screen, 0x00, 0x00, 0x00, 0xFF ); //NEGRO
        SDL_RenderFillRect( screen, &playerRect );
        delete(coordinates);
    }
    // SDL_SetRenderDrawColor(screen, 255, 255, 255, 255);
    //
    // /* Clear the entire screen to our selected color. */
    // SDL_RenderClear(screen);

    SDL_RenderPresent(screen);
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

    SDL_RenderPresent(screen);

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
