#include <iostream>
#include "util/conf.h"

using namespace std;

string Conf::toString()
{
	ostringstream str;

	str << "\nConfiguracion\n";
  str << "Nombre: " << nombre << "\n";
	str << "Formacion: " << formacion << "\n";
	str << "Casaca: " << casaca << "\n";
	str << "Debug Level: " << debugLevel << "\n";
  str << "Framerate: " << framerate << "\n";
  str << "Margenes: " << margenes << "\n";

	return str.str();
}

int chooseDebugLevel(YAML::Node nod)
{
	try{
		if (!nod["debug"]["level"])
			return VALOR_INVALIDO;
		string str = nod["debug"]["level"].as<string>();
		if (!str.compare("debug"))
			return LOG_DEBUG;
		else if (!str.compare("info"))
			return LOG_INFO;
		else if (!str.compare("error"))
			return LOG_ERROR;
		else
			return VALOR_INVALIDO;
	}
	catch (YAML::BadSubscript e) {
		return VALOR_INVALIDO;
	}
}

int chooseCasaca(YAML::Node nod)
{
	try{
		if (!nod["equipo"]["casaca"])
			return VALOR_INVALIDO;
		string str = nod["equipo"]["casaca"].as<string>();
		if (!str.compare("principal"))
			return CASACA_PRINCIPAL;
		else if (!str.compare("alternativa"))
			return CASACA_ALTERNATIVA;
		else
			return VALOR_INVALIDO;
	}
	catch (YAML::BadSubscript e) {
		return VALOR_INVALIDO;
	}
}

int chooseFormacion(YAML::Node nod)
{
	try{
		if (!nod["equipo"]["formacion"])
			return VALOR_INVALIDO;
		string str = nod["equipo"]["formacion"].as<string>();
		if (!str.compare("3-3"))
			return 33;
		else if (!str.compare("3-1-2"))
			return 312;
		else if (!str.compare("3-2-1"))
			return 321;
		// else if (!str.compare("2-3-1"))
		// 	return 231;
		// else if (!str.compare("2-2-2"))
		// 	return 222;
		else
			return VALOR_INVALIDO;
	}
	catch (YAML::BadSubscript e) {
		return VALOR_INVALIDO;
	}
}

string chooseNombre(YAML::Node nod)
{
	try{
		if (!nod["equipo"]["nombre"])
			return "";
		string str = nod["equipo"]["nombre"].as<string>();
		return str;
	}
	catch (YAML::BadSubscript e) {
		return "";
	}
}

int chooseFramerate(YAML::Node nod)
{
	try{
		if (!nod["performance"]["framerate"])
			return VALOR_INVALIDO;
		int str = nod["performance"]["framerate"].as<int>();
		if (str > 0 && str < 300)
			return str;
		else
			return VALOR_INVALIDO;
	}
	catch (YAML::BadSubscript e) {
		return VALOR_INVALIDO;
	}
}

int chooseMargenes(YAML::Node nod)
{
	try{
		if (!nod["performance"]["margenes"])
			return VALOR_INVALIDO;
		int str = nod["performance"]["margenes"].as<int>();
		if (str > 0 && str < 300)
			return str;
		else
			return VALOR_INVALIDO;
	}
	catch (YAML::BadSubscript e) {
		return VALOR_INVALIDO;
	}
}


int Conf::cargarParametro(string parametro, int (*fn)(YAML::Node))
{
	int valor = fn(config);

	if (valor == VALOR_INVALIDO) {
		log("Valor invalido, usando valor por default:", parametro, LOG_ERROR);
		valor = fn(defaultConfig);
	}
	return valor;
}

string Conf::cargarParametro(string parametro, string (*fn)(YAML::Node))
{
	string valor = fn(config);

	if (valor.length() == 0) {
		log("Valor invalido, usando valor por default:", parametro, LOG_ERROR);
		valor = fn(defaultConfig);
	}
	return valor;
}

int Conf::loadConf(string file)
{
	try {
		config = YAML::LoadFile(file);
		log("archivo cargado correctamente:", file, LOG_INFO);
	}
	catch (YAML::BadFile e) {
		log("archivo invalido, cargando default:", file, LOG_ERROR);
		config = YAML::LoadFile(defaultFile);
		//return ARCHIVO_INVALIDO;
	}
	defaultConfig = YAML::LoadFile(defaultFile);
	debugLevel = cargarParametro("Debug level", &chooseDebugLevel);
	log("cargado debug Level con valor:", debugLevel, LOG_INFO);
	casaca = cargarParametro("Casaca", &chooseCasaca);
	log("cargado casaca con valor:", casaca, LOG_INFO);
	formacion = cargarParametro("formacion", &chooseFormacion);
	log("cargado formacion con valor:", formacion, LOG_INFO);
	nombre = cargarParametro("nombre", &chooseNombre);
	log("cargado nombre con valor:", nombre, LOG_INFO);
	margenes = cargarParametro("margenes", &chooseMargenes);
	log("cargado margenes con valor:", margenes, LOG_INFO);
	framerate = cargarParametro("framerate", &chooseFramerate);
	log("cargado framerate con valor:", framerate, LOG_INFO);
	return 0;
}

int Conf::getDebugLevel()
{
	return debugLevel;
}

int Conf::getFormacion()
{
	return formacion;
}

int Conf::getCasaca()
{
	return casaca;
}

string Conf::getNombre()
{
	return nombre;
}
int Conf::getFramerate()
{
	return framerate;
}
int Conf::getMargen()
{
	return margenes;
}

Conf::Conf(string filename)
{
	defaultFile = filename;
}

Conf::~Conf(void)
{

}
