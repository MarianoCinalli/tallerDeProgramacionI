#include "view/PitchView.h"

PitchView::PitchView(Texture* pitch, Camera* camera) {
    this->pitch = pitch;
    this->camera = camera;
}

void PitchView::addPlayerView(PlayerSpriteManager* playerView) {
    this->playerViews.push_back(playerView);
}

void PitchView::render(SDL_Renderer* screen) {
    // Dibujar cancha vista por camara.
    this->renderPitch(screen);
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
}

PitchView::~PitchView() {
    // Libero todos los player views.
    std::list<PlayerSpriteManager*>::iterator it;
    for (it = this->playerViews.begin(); it != this->playerViews.end(); it++) {
        delete(*it);
    }
    // Como la lista sigue llena, de punteros borrados, la vacio.
    this->playerViews.clear();
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
