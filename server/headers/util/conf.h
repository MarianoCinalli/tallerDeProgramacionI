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
    int margenes;
    string defaultFile;
    string defaultSprites;
    int maxClients;
    int port;
    int timePerHalf;
    double playerSpeed;
    double playerMaxSpeed;
    double ballSpeed;
    double ballDecrease;

    Conf(string, string);
    ~Conf();
    int getDebugLevel();
    // string getUsuarios();
    int loadConf(string file);
    int getMargen();
    int getMaxClients();
    int getPort();
    double getPlayerSpeed();
    double getPlayerMaxSpeed();
    double getBallSpeed();
    double getBallDecrease();
    int getTimePerHalf();
    string toString();
    map<string, string> getUsuarios();
  private:

    YAML::Node config;
    YAML::Node defaultConfig;
    map<string, string> usuarios;
    // int chooseUsuarios(YAML::Node nod);
    int debugLevel;
    int cargarParametro(string parametro, int (*fn)(YAML::Node));
    int cargarParametro(string parametro, map<string, string>* usr, int (*fn)(YAML::Node, map<string,string>*));
    double cargarParametroFl(string parametro, double (*fn)(YAML::Node));
    string cargarParametro(string parametro, string (*fn)(YAML::Node));

};

#endif
