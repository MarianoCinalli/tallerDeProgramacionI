#include "util/conf.h"

using namespace std;

string Conf::toString() {
    ostringstream str;

    str << "\nConfiguracion\n";
    str << "Usuarios" << "\n";
    for(auto& x : usuarios)
    {
        str << x.first << " , " << x.second << "\n";
    }
    return str.str();
}

int chooseDebugLevel(YAML::Node nod) {
    try {
        if (!nod["debug"]["level"]) {
            return VALOR_INVALIDO;
        }
        string str = nod["debug"]["level"].as<string>();
        if (!str.compare("debug")) {
            return LOG_DEBUG;
        } else if (!str.compare("info")) {
            return LOG_INFO;
        } else if (!str.compare("error")) {
            return LOG_ERROR;
        } else {
            return VALOR_INVALIDO;
        }
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<int> e) {
        return VALOR_INVALIDO;
    }
}

int chooseUsuarios(YAML::Node nod, map<string, string>* usr) {
    try {
        YAML::Node nodoParametro = nod["usuarios"];
        if (!nodoParametro) {
            return VALOR_INVALIDO;
        }
        for (YAML::Node n : nodoParametro){
          // log("user",n["name"].as<string>()+ " " + n["password"].as<string>(), LOG_DEBUG);
          (*usr)[n["name"].as<string>()] = n["password"].as<string>();
        }
        return VALOR_OK;
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<string> e) {
        return VALOR_INVALIDO;
    }
}

string parametroInvalido(string par) {
    return "Conf: parametro: " + par + " invalido, usando valor por default";
}

int Conf::cargarParametro(string parametro, int (*fn)(YAML::Node)) {
    int valor = fn(config);

    if (valor == VALOR_INVALIDO) {
        log(parametroInvalido(parametro), LOG_ERROR);
        valor = fn(defaultConfig);
    }
    log("Conf: Cargado " + parametro + " con valor:", valor, LOG_INFO);
    return valor;
}

int Conf::cargarParametro(string parametro, map<string, string>* usr, int (*fn)(YAML::Node, map<string,string>*)) {
    int valor = fn(config, usr);

    if (valor == VALOR_INVALIDO) {
        log(parametroInvalido(parametro), LOG_ERROR);
        valor = fn(defaultConfig, usr);
    }
    if(valor == 0){
      log("Conf: Cargado " + parametro + " Ok", LOG_INFO);
    }
    return valor;
}

float Conf::cargarParametro(string parametro, float (*fn)(YAML::Node)) {
    float valor = fn(config);

    if (valor == VALOR_INVALIDO) {
        log(parametroInvalido(parametro), LOG_ERROR);
        valor = fn(defaultConfig);
    }
    log("Conf: Cargado " + parametro + " con valor:", valor, LOG_INFO);
    return valor;
}


string Conf::cargarParametro(string parametro, string(*fn)(YAML::Node)) {
    string valor = fn(config);

    if (valor.length() == 0) {
        log(parametroInvalido(parametro), LOG_ERROR);
        valor = fn(defaultConfig);
    }
    log("Conf: Cargado " + parametro + " con valor:", valor, LOG_INFO);
    return valor;
}

int Conf::loadConf(string file) {
    try {
        config = YAML::LoadFile(file);
        log("Conf: Archivo cargado correctamente: ", file, LOG_INFO);
    } catch (YAML::BadFile e) {
        log("Conf: Archivo invalido, cargando default: ", file, LOG_ERROR);
        config = YAML::LoadFile(defaultFile);
    } catch (YAML::ParserException e) {
        log("Conf: Archivo invalido, cargando default: ", file, LOG_ERROR);
        config = YAML::LoadFile(defaultFile);
    }

    defaultConfig = YAML::LoadFile(defaultFile);
    debugLevel = cargarParametro("Debug level", &chooseDebugLevel);
    log("Conf: Cargado debug Level con valor: ", getMessageLevelString(debugLevel), LOG_INFO);
    cargarParametro("usuarios", &this->usuarios,&chooseUsuarios);
    return 0;
}

int Conf::getDebugLevel() {
    return debugLevel;
}


Conf::Conf(string filename, string defaultSpritesFilename) {
    defaultFile = filename;
    defaultSprites = defaultSpritesFilename;
}

Conf::~Conf(void) {

}
