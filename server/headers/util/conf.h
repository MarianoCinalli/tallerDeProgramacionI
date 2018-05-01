#include <iostream>
#include <yaml-cpp/yaml.h>
#include <unistd.h>
#include "util/Constants.h"
#include "util/logger.h"
#ifndef CONF_H
#define CONF_H

using namespace std;

// enum valorInvalido {
//   VALOR_INVALIDO = 5,
//   ARCHIVO_INVALIDO = 6
// };

enum casacas {
  PRINCIPAL = 1,
  ALTERNATIVA = 2,
};

class Conf : public Loggable {
  public:
    Conf(string, string);
    ~Conf();
    string defaultFile;
    string defaultSprites;


    int getDebugLevel();
    // string getUsuarios();
    int loadConf(string file);
    string toString();
  private:

    YAML::Node config;
    YAML::Node defaultConfig;
    map<string, string> usuarios;
    // int chooseUsuarios(YAML::Node nod);
    int debugLevel;
    int cargarParametro(string parametro, int (*fn)(YAML::Node));
    int cargarParametro(string parametro, map<string, string>* usr, int (*fn)(YAML::Node, map<string,string>*));
    float cargarParametro(string parametro, float (*fn)(YAML::Node));
    string cargarParametro(string parametro, string (*fn)(YAML::Node));

};

#endif
