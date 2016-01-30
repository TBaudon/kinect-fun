
#ifndef DATAHANDLER_H_
#define DATAHANDLER_H_

#include <stdint.h>

class DataHandler {

public:

	/**
	 * Constructeur
	 */
	DataHandler();

	/**
	 * Destructeur
	 */
	virtual ~DataHandler();

	/**
	 * Fonction de traitement des données de profondeur
	 * Les données arrivent par le tableau en parametre
	 * de dimension 640*480
	 */
	virtual void depthFunc(uint16_t* depth);

	/**
	 * Fonction de traitement des données de couleurs
	 * Les données arrivent par le tableau en parametre
	 * de dimension 3*640*480 (un octet par composante de couleure)
	 */
	virtual void rgbFunc(uint8_t* rgb);

};

#endif /* DATAHANDLER_H_ */
