#include "util/Constants.h"
#include <string>
#include "interfaces/Loggable.h"
#include <stdint.h>

#ifndef COLOUR_H
#define COLOUR_H

class Colour : public Loggable {
    private:
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
        std::string toString();
    public:
        Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
        uint8_t getRed();
        uint8_t getGreen();
        uint8_t getBlue();
        uint8_t getAlpha();
        ~Colour();
};
#endif // COLOUR_H


