#include "util/Colour.h"

Colour::Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
}

uint8_t Colour::getRed() {
    return this->red;
}

uint8_t Colour::getGreen() {
    return this->green;
}

uint8_t Colour::getBlue() {
    return this->blue;
}

uint8_t Colour::getAlpha() {
    return this->alpha;
}

std::string Colour::toString() {
    return "Rojo: " + std::to_string(this->red) + " Verde: " + std::to_string(this->green) + " Azul: " + std::to_string(this->blue);
}

Colour::~Colour() {
}
