#include <iostream>
#include <yaml-cpp/yaml.h>
#include "util/Constants.h"
#include "util/logger.h"
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

class Conf : public Loggable {
  public:
    Conf();
    ~Conf();
    const std::string defaultFile = "src/default.yaml";
    int getFormacion();
    int getCasaca();    //principal true o alternativa false
    int getDebugLevel();
    int loadConf(string file);
    string toString();
  private:

    YAML::Node config;
    YAML::Node defaultConfig;
    int formacion;
    int casaca;    //principal true o alternativa false
    int debugLevel;
    void cargarCasaca();
    void cargarFormacion();
    void cargarDebug();
    int cargarParametro(string parametro, int (*fn)(YAML::Node));


};

#endif
