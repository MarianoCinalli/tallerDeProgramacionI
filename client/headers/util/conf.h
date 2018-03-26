#include <iostream>
#include <yaml-cpp/yaml.h>
#include "util/Constants.h"
#ifndef CONF_H
#define CONF_H

using namespace std;

enum valorInvalido {
  VALOR_INVALIDO = 5
};

enum casacas {
  PRINCIPAL = 1,
  ALTERNATIVA = 2,
};

class Conf {
  public:

    string getFormacion();
    int getCasaca();    //principal true o alternativa false
    int getDebugLevel();
    int loadConf(string file);

    void printConf();
  private:
    YAML::Node config;
    string formacion;
    int casaca;    //principal true o alternativa false
    int debugLevel;

};

#endif
