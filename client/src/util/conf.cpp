#include <iostream>
#include "util/conf.h"

using namespace std;

int Conf::loadConf(string file){
  try {
    config = YAML::LoadFile(file);
  }
  catch (YAML::BadFile e) {
   printf("bad file\n");
   return 1;
  }
  debugLevel = config["debug"]["level"].as<std::string>();
  formacion = config["equipo"]["formacion"].as<std::string>();
  casaca = config["equipo"]["casaca"].as<std::string>();
  return 0;
}

void Conf::printConf(){
  cout << "Debug Level: " << debugLevel << "\n";
  cout << "Formacion: "<< formacion << "\n";
  cout << "Casaca: " <<  casaca << "\n";
}
