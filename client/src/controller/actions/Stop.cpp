#include "controller/actions/Stop.h"

Stop::Stop() {
}

std::string Stop::getCommand() {
    return "Stop";
}

std::string Stop::getParameters() {
    return "";
}
Stop::~Stop() {
}
