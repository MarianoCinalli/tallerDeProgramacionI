#include "util/conf.h"

using namespace std;

const int VALOR_INVALIDO = -1;

string Conf::toString() {
    ostringstream str;

    str << "\nConfiguracion\n";
    str << "Usuarios" << "\n";
    for (auto& x : usuarios) {
        str << x.first << " , " << x.second << "\n";
    }
    str << "Cantidad maxima de clientes: " << this->maxClients << "\n";
    str << "Puerto a escuchar: " << this->port << "\n";
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

int chooseUsuarios(YAML::Node nod, map<string, string>* usr) {
    try {
        YAML::Node nodoParametro = nod["usuarios"];
        if (!nodoParametro) {
            return VALOR_INVALIDO;
        }
        for (YAML::Node n : nodoParametro) {
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

int chooseMaxClients(YAML::Node nod) {
    try {
        if (!nod["server"]["maxClients"]) {
            return VALOR_INVALIDO;
        }
        int str = nod["server"]["maxClients"].as<int>();
        if (str > 0 && str < 5) {
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

int choosePort(YAML::Node nod) {
    try {
        if (!nod["server"]["port"]) {
            return VALOR_INVALIDO;
        }
        int str = nod["server"]["port"].as<int>();
        return str;
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<int> e) {
        return VALOR_INVALIDO;
    }
}

int chooseTimePerHalf(YAML::Node nod) {
    try {
        if (!nod["juego"]["minutosPorTiempo"]) {
            return VALOR_INVALIDO;
        }
        int str = nod["juego"]["minutosPorTiempo"].as<int>();
        return str;
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<int> e) {
        return VALOR_INVALIDO;
    }
}

double choosePlayerSpeed(YAML::Node nod) {
    try {
        if (!nod["jugador"]["velocidad"]) {
            return VALOR_INVALIDO;
        }
        double str = nod["jugador"]["velocidad"].as<double>();
        return str;
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<double> e) {
        return VALOR_INVALIDO;
    }
}

double choosePlayerMaxSpeed(YAML::Node nod) {
    try {
        if (!nod["jugador"]["masRapido"]) {
            return VALOR_INVALIDO;
        }
        double str = nod["jugador"]["masRapido"].as<double>();
        return str;
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<double> e) {
        return VALOR_INVALIDO;
    }
}

double chooseBallSpeed(YAML::Node nod) {
    try {
        if (!nod["pelota"]["velocidad"]) {
            return VALOR_INVALIDO;
        }
        double str;
        str = nod["pelota"]["velocidad"].as<double>();
        return str;
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<double> e) {
        return VALOR_INVALIDO;
    }
}

double chooseBallDecrease(YAML::Node nod) {
    try {
        if (!nod["pelota"]["desacelerar"]) {
            return VALOR_INVALIDO;
        }
        double str = nod["pelota"]["desacelerar"].as<double>();
        return str;
    } catch (YAML::BadSubscript e) {
        return VALOR_INVALIDO;
    } catch (YAML::TypedBadConversion<double> e) {
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

int Conf::cargarParametro(string parametro, map<string, string>* usr, int (*fn)(YAML::Node, map<string, string>*)) {
    int valor = fn(config, usr);

    if (valor == VALOR_INVALIDO) {
        log(parametroInvalido(parametro), LOG_ERROR);
        valor = fn(defaultConfig, usr);
    }
    if (valor == 0) {
        log("Conf: Cargado " + parametro + " Ok", LOG_INFO);
    }
    return valor;
}

double Conf::cargarParametroFl(string parametro, double (*fn)(YAML::Node)) {
    double valor = fn(config);

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
    margenes = cargarParametro("margenes", &chooseMargenes);
    log("Conf: Cargado margenes con valor: ", margenes, LOG_INFO);
    this->maxClients = cargarParametro("maxClients", &chooseMaxClients);
    log("Conf: Cargado la cantidad maxima de jugadores con valor: ", this->maxClients, LOG_INFO);
    this->port = cargarParametro("puerto", &choosePort);
    log("Conf: Cargado el puerto con valor: ", this->port, LOG_INFO);
    this->timePerHalf = cargarParametro("minutosPorTiempo", &chooseTimePerHalf);
    log("Conf: Cargado la duracion de cada mitad con valor: ", this->timePerHalf, LOG_INFO);
    this->playerSpeed = cargarParametroFl("velocidadJugador", &choosePlayerSpeed);
    log("Conf: Cargado la velocidad del jugador con valor: ", this->playerSpeed, LOG_INFO);
    this->playerMaxSpeed = cargarParametroFl("velocidadMaximaJugador", &choosePlayerMaxSpeed);
    log("Conf: Cargado la velocidad maxima del jugador con valor: ", this->playerMaxSpeed, LOG_INFO);
    this->ballSpeed = cargarParametroFl("velocidadPelota", &chooseBallSpeed);
    log("Conf: Cargado la velocidad de la pelota con valor: ", this->ballSpeed, LOG_INFO);
    this->ballDecrease = cargarParametroFl("desacelerarPelota", &chooseBallDecrease);
    log("Conf: Cargado la desaceleracion de la pelota con valor: ", this->ballDecrease, LOG_INFO);


    cargarParametro("usuarios", &this->usuarios, &chooseUsuarios);
    return 0;
}

int Conf::getDebugLevel() {
    return debugLevel;
}

int Conf::getMargen() {
    return margenes;
}

int Conf::getMaxClients() {
    return this->maxClients;
}

int Conf::getPort() {
    return this->port;
}

int Conf::getTimePerHalf() {
    return this->timePerHalf;
}

double Conf::getPlayerSpeed(){
  return this->playerSpeed;
}

double Conf::getPlayerMaxSpeed(){
  return this->playerMaxSpeed;
}

double Conf::getBallSpeed(){
  return this->ballSpeed;
}

double Conf::getBallDecrease(){
  return this->ballDecrease;
}

map<string, string> Conf::getUsuarios(){
  return this->usuarios;
}

Conf::Conf(string filename, string defaultSpritesFilename) {
    defaultFile = filename;
    defaultSprites = defaultSpritesFilename;
}

Conf::~Conf(void) {

}
