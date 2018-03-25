#include <iostream>
#include <yaml-cpp/yaml.h>
#ifndef CONF_H
#define CONF_H


using namespace std;

class Conf {
  public:
    string formacion;
    string casaca;    //principal true o alternativa false
    string debugLevel;

    int loadConf(string file);
    void printConf();
  private:
    YAML::Node config;
};

#endif
