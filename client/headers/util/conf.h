#include <iostream>
#include <yaml-cpp/yaml.h>
#include "util/Constants.h"
#ifndef CONF_H
#define CONF_H

using namespace std;

enum valorInvalido {
  VALOR_INVALIDO = 5,
  ARCHIVO_INVALIDO = 6
};

enum casacas {
  PRINCIPAL = 1,
  ALTERNATIVA = 2,
};

class Conf {
  public:

    int getFormacion();
    int getCasaca();    //principal true o alternativa false
    int getDebugLevel();
    int loadConf(string file);

    void printConf();
  private:
    const std::string defaultFile = "src/default.yaml";
    YAML::Node config;
    YAML::Node defaultConfig;
    int formacion;
    int casaca;    //principal true o alternativa false
    int debugLevel;
    void cargarCasaca();
    void cargarFormacion();
    void cargarDebug();


};

#endif
