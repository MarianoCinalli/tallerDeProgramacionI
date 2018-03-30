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

int chooseFormacion(string str) {
  if (!str.compare("3-3")) {
          return 33;
  }
  else if (!str.compare("3-1-2")) {
          return 312;
  }
  else if (!str.compare("3-2-1")) {
          return 321;
  }
  else if (!str.compare("2-3-1")) {
          return 231;
  }
  else if (!str.compare("2-2-2")) {
          return 222;
  }
  else
          return VALOR_INVALIDO;
}

void Conf::cargarDebug(){
  string parametro = "level debug";
  if(!config["debug"]["level"]){
      printf("no hay %s\n", parametro.c_str());
      debugLevel = chooseDebugLevel(defaultConfig["debug"]["level"].as<std::string>());
  }
  else {
      debugLevel = chooseDebugLevel(config["debug"]["level"].as<std::string>());
      if (debugLevel == VALOR_INVALIDO){
          printf("%s invalido, valor por default\n", parametro.c_str());
          debugLevel = chooseDebugLevel(defaultConfig["debug"]["level"].as<std::string>());
        }
  }
}

void Conf::cargarFormacion(){
  string parametro = "formacion";
  if(!config["equipo"]["formacion"]){
    printf("no hay %s\n", parametro.c_str());
      formacion = chooseFormacion(defaultConfig["equipo"]["formacion"].as<std::string>());
  }
  else {
    formacion = chooseFormacion(config["equipo"]["formacion"].as<std::string>());
    if (formacion == VALOR_INVALIDO){
      printf("%s invalido, valor por default\n", parametro.c_str());
        formacion = chooseFormacion(defaultConfig["equipo"]["formacion"].as<std::string>());
        }
  }
}

void Conf::cargarCasaca(){
  string parametro = "casaca";
  if(!config["equipo"]["casaca"]){
    printf("no hay %s\n", parametro.c_str());
      casaca = chooseCasaca(defaultConfig["equipo"]["casaca"].as<std::string>());
  }
  else {
    casaca = chooseCasaca(config["equipo"]["casaca"].as<std::string>());
    if (casaca == VALOR_INVALIDO){
      printf("%s invalido, valor por default\n", parametro.c_str());
      casaca = chooseCasaca(defaultConfig["equipo"]["casaca"].as<std::string>());
        }
  }
}
int Conf::loadConf(string file){
        try {
                config = YAML::LoadFile(file);
        }
        catch (YAML::BadFile e) {
                printf("archivo invalido, cargando default\n");
                config = YAML::LoadFile(defaultFile);
                //return ARCHIVO_INVALIDO;
        }
        defaultConfig = YAML::LoadFile(defaultFile);
        cargarDebug();
        cargarFormacion();
        cargarCasaca();
        return 0;
}

int Conf::getDebugLevel() {
        return debugLevel;
}

int Conf::getFormacion(){
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
