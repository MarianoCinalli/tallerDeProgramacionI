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

int cargarParametro(YAML::Node conf, YAML::Node defaultConf, string parametro, int (*fn)(string)){
  int valor;
  if(!conf){
    printf("no hay %s\n", parametro.c_str());
      valor = fn(defaultConf.as<std::string>());
  }
  else {
    valor = fn(conf.as<std::string>());
    if (valor == VALOR_INVALIDO){
      printf("%s invalido, valor por default\n", parametro.c_str());
      valor = fn(conf.as<std::string>());
        }
  }
  return valor;
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
        casaca = cargarParametro(config["equipo"]["casaca"], defaultConfig["equipo"]["casaca"], "Casaca", &chooseCasaca);
        formacion = cargarParametro(config["equipo"]["formacion"], defaultConfig["equipo"]["Formacion"], "formacion", &chooseFormacion);
        debugLevel = cargarParametro(config["debug"]["level"], defaultConfig["debug"]["level"], "Debug level", &chooseDebugLevel);
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
