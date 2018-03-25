#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <yaml-cpp/yaml.h>
#include "util/Constants.h"
#include "util/logger.h"
<<<<<<< HEAD
#include "controller/cancha_controller.h"
//#include <yaml.h>
=======
#include "util/conf.h"

>>>>>>> Implementada primera version del parser, en la clase Conf

// Global variables ---------------------------------------
int LOG_MIN_LEVEL = LOG_ERROR; // Cambiar por parametro parseado.
std::ofstream LOG_FILE_POINTER;
Conf conf;
const std::string confFileName = "src/config.yaml";
const std::string logFileName = "trabajoPractico.log";
// Global variables ---------------------------------------

void cargarConfiguracion(){
  if (conf.loadConf("src/config.yaml")){
    conf.loadConf(confFileName); //Configuracion buena si falla la primera
  }
  else {
    conf.printConf();
  }
}

int main(int argc, char* argv[]) {
    // Log initialization ---------------------------------
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    logSessionStarted();
    // Program
    startView();
    if (conf.loadConf("src/config.yaml")){
      conf.loadConf(confFileName); //Configuracion buena si falla
    }
    else {
      conf.printConf();
    }
    cargarConfiguracion();
    logSessionFinished();
    LOG_FILE_POINTER.close();

    return 0;
}
