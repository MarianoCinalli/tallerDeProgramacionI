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

void PitchView::render(SDL_Renderer* screen) {
    // Dibujar cancha vista por camara.
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
    SDL_Rect rect = this->camera->getRectToDraw();
    SDL_RenderCopy(
        screen,
        this->pitch->getSpriteSheetTexture(),
        &rect,
        NULL
    );
}
