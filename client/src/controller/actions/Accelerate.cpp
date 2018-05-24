#include "controller/actions/Accelerate.h"

Accelerate::Accelerate(int direction) {
    this->direction = direction;
}

std::string Accelerate::getCommand() {
    return "Accelerate";
}

std::string Accelerate::getParameters() {
    return std::to_string(this->direction);
}

Accelerate::~Accelerate() {
}
