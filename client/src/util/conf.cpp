#include <iostream>
#include "util/conf.h"

using namespace std;

int chooseDebugLevel(string str) {
        if (!str.compare("debug")) {
                return LOG_DEBUG;
        }
        else if (!str.compare("info")) {
                return LOG_INFO;
        }
        else if (!str.compare("error")) {
                return LOG_ERROR;
        }
        else
                return VALOR_INVALIDO;
}

int chooseCasaca(string str) {
        if (!str.compare("principal")) {
                return LOG_DEBUG;
        }
        else if (!str.compare("alternativa")) {
                return LOG_INFO;
        }
        else
                return VALOR_INVALIDO;
}

int Conf::loadConf(string file){
        try {
                config = YAML::LoadFile(file);
        }
        catch (YAML::BadFile e) {
                printf("bad file\n");
                return 1;
        }
        debugLevel = chooseDebugLevel(config["debug"]["level"].as<std::string>());
        formacion = config["equipo"]["formacion"].as<std::string>();
        casaca = chooseCasaca(config["equipo"]["casaca"].as<std::string>());
        if((debugLevel == VALOR_INVALIDO)|| (casaca == VALOR_INVALIDO)) { //falta poner la validacion de la formacion
                printf("bad conf\n");
                return 1;
        }
        return 0;
}

int Conf::getDebugLevel() {
        return debugLevel;
}

string Conf::getFormacion(){
        return formacion;
}

int Conf::getCasaca() {
        return casaca;
}

void Conf::printConf(){
        cout << "Debug Level: " << getDebugLevel() << "\n";
        cout << "Formacion: "<< getFormacion() << "\n";
        cout << "Casaca: " <<  getCasaca() << "\n";
}
