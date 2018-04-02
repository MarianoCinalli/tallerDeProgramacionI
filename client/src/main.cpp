#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <yaml-cpp/yaml.h>
#include "util/Constants.h"
#include "util/logger.h"
#include "util/conf.h"
#include "controller/CanchaController.h"

// Global variables ---------------------------------------
int LOG_MIN_LEVEL = LOG_DEBUG; // Cambiar por parametro parseado.
std::ofstream LOG_FILE_POINTER;

const std::string logFileName = "trabajoPractico.log";
const std::string defaultConfFileName = "src/default.yaml";
const std::string confFileName = "conf.yaml";

Conf conf(defaultConfFileName);

void cargarConfiguracion(string confFile){
  conf.loadConf(confFile);
}

int main(int argc, char* argv[]) {
    // Log initialization ---------------------------------
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    logSessionStarted();

    //Configuracion
    cargarConfiguracion(confFileName);
    log(&conf, LOG_INFO);

    // Program
    CanchaController canchaController;
    canchaController.startView();

    logSessionFinished();
    LOG_FILE_POINTER.close();

    return 0;
}
