#include <iostream>
#include "util/conf.h"

using namespace std;

int chooseDebugLevel(YAML::Node nod) {
        try{
        if (!nod["debug"]["level"]){
          return VALOR_INVALIDO;
        }
        string str = nod["debug"]["level"].as<string>();
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
        catch (YAML::BadSubscript e){
          return VALOR_INVALIDO;
        }

}

int chooseCasaca(YAML::Node nod) {
        try{
          if (!nod["equipo"]["casaca"]){
            return VALOR_INVALIDO;
          }
          string str = nod["equipo"]["casaca"].as<string>();
          if (!str.compare("principal")) {
                  return LOG_DEBUG;
          }
          else if (!str.compare("alternativa")) {
                  return LOG_INFO;
          }
          else
                  return VALOR_INVALIDO;
        }
        catch (YAML::BadSubscript e){
          return VALOR_INVALIDO;
        }
}

int chooseFormacion(YAML::Node nod) {
  try{
    if (!nod["equipo"]["formacion"]){
      return VALOR_INVALIDO;
    }
    string str = nod["equipo"]["formacion"].as<string>();
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
    catch (YAML::BadSubscript e){
      return VALOR_INVALIDO;
    }
}

int Conf::cargarParametro(string parametro, int (*fn)(YAML::Node)){
  int valor = fn(config);
  if(valor == VALOR_INVALIDO){
    printf("%s invalido, valor por default\n", parametro.c_str());
    valor = fn(defaultConfig);
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
        debugLevel = cargarParametro("Debug level", &chooseDebugLevel);
        casaca = cargarParametro("Casaca", &chooseCasaca);
        formacion = cargarParametro("formacion", &chooseFormacion);
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
