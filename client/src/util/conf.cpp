#include "util/conf.h"

using namespace std;

string Conf::toString() {
    ostringstream str;

    str << "\nConfiguracion\n";
    str << "Nombre: " << nombre << "\n";
    str << "Formacion: " << formacion << "\n";
    string casacaStr;
    if (casaca == CASACA_PRINCIPAL) {
        casacaStr = "Principal";
    } else {
        casacaStr = "Alternativa";
    }
    str << "Casaca: " << casacaStr << "\n";
    str << "Shirts Path: " << spritesPath << "\n";
    str << "Debug Level: " << debugLevel << "\n";
    str << "Framerate: " << framerate << "\n";
    str << "Margenes: " << margenes << "\n";

    return str.str();
}

int chooseDebugLevel(YAML::Node nod) {
    try {
        if (!nod["debug"]["level"]) {
            return VALOR_INVALIDO;
        }
        string str = nod["debug"]["level"].as<string>();
        return getLogLevelFromString(str);
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<int> e) {
        return VALOR_INVALIDO;
    }
}

int chooseCasaca(YAML::Node nod) {
    try {
        if (!nod["equipo"]["casaca"]) {
            return VALOR_INVALIDO;
        }
        string str = nod["equipo"]["casaca"].as<string>();
        if (!str.compare("principal")) {
            return CASACA_PRINCIPAL;
        } else if (!str.compare("alternativa")) {
            return CASACA_ALTERNATIVA;
        } else {
            return VALOR_INVALIDO;
        }
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<int> e) {
        return VALOR_INVALIDO;
    }
}

int chooseFormacion(YAML::Node nod) {
    try {
        if (!nod["equipo"]["formacion"]) {
            return VALOR_INVALIDO;
        }
        string str = nod["equipo"]["formacion"].as<string>();
        if (!str.compare("3-3")) {
            return 33;
        } else if (!str.compare("3-1-2")) {
            return 312;
        } else if (!str.compare("3-2-1")) {
            return 321;
        } else {
            return VALOR_INVALIDO;
        }
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<int> e) {
        return VALOR_INVALIDO;
    }
}

string chooseNombre(YAML::Node nod) {
    try {
        if (!nod["equipo"]["nombre"]) {
            return "";
        }
        string str = nod["equipo"]["nombre"].as<string>();
        return str;
    } catch (YAML::BadSubscript e) {
        return "";
    } catch (YAML::TypedBadConversion<string> e) {
        return "";
    }
}

string chooseSpritesPath(YAML::Node nod) {
    try {
        if (!nod["assets"]["sprites"]) {
            return "";
        }
        string str = nod["assets"]["sprites"].as<string>();
        //chequea que el archivo exista
        int res = access(str.c_str(), R_OK);
        if (res < 0) {
            return "invalid file";
        }
        return str;
    } catch (YAML::BadSubscript e) {
        return "";
    } catch (YAML::TypedBadConversion<string> e) {
        return "";
    }
}

float chooseFramerate(YAML::Node nod) {
    try {
        if (!nod["performance"]["framerate"]) {
            return VALOR_INVALIDO;
        }
        float str = nod["performance"]["framerate"].as<float>();
        if (str > 0 && str < 300) { //harcode
            return str;
        } else {
            return VALOR_INVALIDO;
        }
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<float> e) {
        return VALOR_INVALIDO;
    }
}

int chooseMargenes(YAML::Node nod) {
    try {
        if (!nod["performance"]["margenes"]) {
            return VALOR_INVALIDO;
        }
        int str = nod["performance"]["margenes"].as<int>();
        if (str > 0 && str < 300) {
            return str;
        } else {
            return VALOR_INVALIDO;
        }
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<int> e) {
        return VALOR_INVALIDO;
    }
}

string parametroInvalido(string par) {
    return "Conf: parametro: " + par + " invalido, usando valor por default";
}

int Conf::cargarParametro(string parametro, int (*fn)(YAML::Node)) {
    int valor = fn(config);

    if (valor == VALOR_INVALIDO) {
        // log("Conf: Valor invalido, usando valor por default: ", parametro, LOG_ERROR);
        log(parametroInvalido(parametro), LOG_ERROR);
        valor = fn(defaultConfig);
    }
    return valor;
}

float Conf::cargarParametro(string parametro, float (*fn)(YAML::Node)) {
    float valor = fn(config);

    if (valor == VALOR_INVALIDO) {
        log(parametroInvalido(parametro), LOG_ERROR);
        valor = fn(defaultConfig);
    }
    return valor;
}


string Conf::cargarParametro(string parametro, string defaultPath, string(*fn)(YAML::Node)) {
    string valor = fn(config);

    if (valor.length() == 0) {
        log(parametroInvalido(parametro), LOG_ERROR);
        valor = fn(defaultConfig);
    }
    if (valor == "invalid file") {
        log(parametroInvalido(parametro), LOG_ERROR);
        return defaultPath;
    }
    return valor;
}

int Conf::loadConf(string file) {
    try {
        config = YAML::LoadFile(file);
        log("Conf: Archivo cargado correctamente: ", file, LOG_INFO);
    } catch (YAML::BadFile e) {
        log("Conf: Archivo invalido, cargando default: ", file, LOG_ERROR);
        config = YAML::LoadFile(defaultFile);
        //return ARCHIVO_INVALIDO;
    } catch (YAML::ParserException e) {
        log("Conf: Archivo invalido, cargando default: ", file, LOG_ERROR);
        config = YAML::LoadFile(defaultFile);
        //return ARCHIVO_INVALIDO;
    }

    defaultConfig = YAML::LoadFile(defaultFile);
    debugLevel = cargarParametro("Debug level", &chooseDebugLevel);
    log("Conf: Cargado debug Level con valor: ", getMessageLevelString(debugLevel), LOG_INFO);
    casaca = cargarParametro("Casaca", &chooseCasaca);
    log("Conf: Cargado casaca con valor: ", casaca, LOG_INFO);
    formacion = cargarParametro("formacion", &chooseFormacion);
    log("Conf: Cargado formacion con valor: ", formacion, LOG_INFO);
    nombre = cargarParametro("nombre", "zidane", &chooseNombre);
    log("Conf: Cargado nombre con valor:", nombre, LOG_INFO);
    spritesPath = cargarParametro("spritesPath", this->defaultSprites, &chooseSpritesPath);
    log("Conf: Cargado shirtsPath con valor:", spritesPath, LOG_INFO);
    margenes = cargarParametro("margenes", &chooseMargenes);
    log("Conf: Cargado margenes con valor: ", margenes, LOG_INFO);
    framerate = cargarParametro("framerate", &chooseFramerate);
    log("Conf: Cargado framerate con valor: ", framerate, LOG_INFO);
    return 0;
}

int Conf::getDebugLevel() {
    return debugLevel;
}

string Conf::getAssetsCancha() {
    return "images/cancha.png"; //TODO
}

string Conf::getAssetsCanchaMini() {
    return "images/minimap.png"; //TODO
}

int Conf::getFormacion() {
    return formacion;
}

int Conf::getCasaca() {
    return casaca;
}

string Conf::getNombre() {
    return nombre;
}

string Conf::getSpritesPath() {
    return spritesPath;
}

float Conf::getFramerate() {
    return framerate;
}
int Conf::getMargen() {
    return margenes;
}

Conf::Conf(string filename, string defaultSpritesFilename) {
    defaultFile = filename;
    defaultSprites = defaultSpritesFilename;
}

Conf::~Conf(void) {

}
