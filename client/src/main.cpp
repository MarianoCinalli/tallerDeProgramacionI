#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <yaml-cpp/yaml.h>
#include <getopt.h>
#include "util/Constants.h"
#include "util/logger.h"
#include "util/conf.h"
#include "controller/CanchaController.h"



void imprimir_ayuda()
{
	printf("Usage:\n");
	printf("main -h\n");
	printf("main -V\n");
	printf("main [options]\n");
	printf("-V, --version		Imprimir version y salir.\n");
	printf("-h, --help		Imprimir esta ayuda.\n");
	printf("-i, --input		Path del archivo de configuracion YAML.\n");
	printf("Ejemplo:\n");
	printf("tp0 -i ~/conf.yaml \n");
}

void imprimir_version()
{
	printf("Version = 1.0000000000001\n");
}

// Global variables ---------------------------------------
std::ofstream LOG_FILE_POINTER;

const std::string logFileName = "trabajoPractico.log";
const std::string defaultConfFileName = "src/default.yaml";
std::string confFileName;
int LOG_MIN_LEVEL;
Conf conf(defaultConfFileName);

void cargarConfiguracion(string confFile){
  conf.loadConf(confFile);
  //LOG_MIN_LEVEL = conf.getDebugLevel(); // Cambiar por parametro parseado.
}

int chequearOpciones(int argc, char* argv[]){
  int ch;
  while (1)
    {
      static struct option long_options[] =
      {
        //Flags posibles
        {"version", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {"input", required_argument, 0, 'i'},

        {0, 0, 0, 0}
      };

      int option_index = 0;

      ch = getopt_long (argc, argv, "vhi:",
               long_options, &option_index);

      /* Detecta fin de opciones. */
      if (ch == -1){
        break;
      }

      switch (ch)
      {
      case 0:
        if (long_options[option_index].flag != 0) {
          break;
        }
        printf ("Opcion %s", long_options[option_index].name);
        if (optarg) {
          printf (" con argumentos %s", optarg);
          printf ("\n");
          break;
        }

      case 'v':
        imprimir_version();
        return 1;
        break;

      case 'h':
        imprimir_ayuda();
        return 1;
        break;

      case 'i':
  					confFileName = optarg;
            return 0;
  		break;

      default:
        printf("Opcion invalida\n\n");
        return 1;
      }
  }
  confFileName = "conf.yaml";
  return 0;
}

int main(int argc, char* argv[]) {

    if(chequearOpciones(argc, argv)){
      return SALIDA_LINEA_COMANDOS;
    }
    // Log initialization ---------------------------------
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    // logSessionStarted();

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
