#include "view/PitchView.h"

PitchView::PitchView() {
}

void PitchView::addPlayerView(PlayerSpriteManager* playerView) {
    this->playerViews.push_back(playerView);
}

void PitchView::render() {
    // Dibujar cancha vista por camara.
    // std::list<PlayerSpriteManager*> views = this->camara->getPlayersInside(this->playerViews);
    // std::list<PlayerSpriteManager*>::iterator viewIter;
    // for (viewIter = views.begin(); viewIter != views.end(); viewIter++) {
           // Coordinates* coordinates = this->camara->getRelativeCoordinates(*viewIter);
           //(*viewIter)->render(renderer, coordinates);
    // }
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
