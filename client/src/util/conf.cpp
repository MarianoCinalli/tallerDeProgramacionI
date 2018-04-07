#include <iostream>
#include "util/conf.h"

using namespace std;

string Conf::toString(){
  ostringstream str;

  str << "\nConfiguracion\n";
  str << "Formacion: " << formacion << "\n";
  str << "Casaca: " << casaca << "\n";
  str << "Debug Level: " << debugLevel << "\n";

  return str.str();
}

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
  log("Valor invalido, usando valor por default:", parametro.c_str(),LOG_ERROR);
    valor = fn(defaultConfig);
  }
  return valor;
}

int Conf::loadConf(string file){
        try {
                config = YAML::LoadFile(file);
                log("archivo cargado correctamente:", file.c_str(), LOG_INFO);
        }
        catch (YAML::BadFile e) {
              log("archivo invalido, cargando default:",file.c_str(), LOG_ERROR);
                config = YAML::LoadFile(defaultFile);
                //return ARCHIVO_INVALIDO;
        }
        defaultConfig = YAML::LoadFile(defaultFile);
        debugLevel = cargarParametro("Debug level", &chooseDebugLevel);
        log("cargado debug Level con valor:",debugLevel, LOG_INFO);
        casaca = cargarParametro("Casaca", &chooseCasaca);
        log("cargado casaca con valor:",casaca, LOG_INFO);
        formacion = cargarParametro("formacion", &chooseFormacion);
        log("cargado formacion con valor:",formacion, LOG_INFO);
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

Conf::Conf(string filename){
  defaultFile = filename;
}

Conf::~Conf(void){

}
