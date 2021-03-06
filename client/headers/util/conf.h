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
    string nombre;
    string spritesPath;
    float framerate;
    int margenes;
    int getFormacion();
    int getCasaca();    //principal true o alternativa false
    int getDebugLevel();
    string getAssetsCancha();
    string getAssetsCanchaMini();
    string getNombre();
    string getSpritesPath();
    // string chooseShirtsPath(YAML::Node nod);
    float getFramerate();
    int getMargen();
    int loadConf(string file);
    string toString();
  private:

    YAML::Node config;
    YAML::Node defaultConfig;
    int formacion;
    int casaca;    //principal true o alternativa false
    int debugLevel;
    int cargarParametro(string parametro, int (*fn)(YAML::Node));
    float cargarParametro(string parametro, float (*fn)(YAML::Node));
    string cargarParametro(string parametro, string parametro2, string (*fn)(YAML::Node));

};

#endif
