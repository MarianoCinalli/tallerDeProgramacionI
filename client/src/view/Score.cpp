#include "view/Score.h"

Score::Score() {
    log("Score: Creando score...", LOG_INFO);
}

void Score::parseYaml(YAML::Node node){

}

Score::~Score() {
    log("Score: Liberando memoria...", LOG_INFO);
}
