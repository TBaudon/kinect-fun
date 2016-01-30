/*
 * Kinect.h
 *
 *  Created on: 14 févr. 2012
 *      Author: thomas
 */

#ifndef KINECT_H_
#define KINECT_H_

#include <libfreenect.hpp>
#include <boost/thread.hpp>

class DataHandler;

class Kinect {

public:

	/**
	 * Recupéreration de l'instance
	 */
	static Kinect* open();

	/**
	 * Détrui linstance du singleton et ferme le kinect
	 */
	void close();

	/**
	 * On lance la capture video dans un nouveau thread
	 */
	void startCapture();

	/**
	 * On stop la capture et le thread
	 */
	void stopCapture();

	/**
	 * Thread de capture des données
	 */
	void captureThread();

	/**
	 * Definie le pointeur vers le datahandler pour pouvoir
	 * en utiliser les methodes rgbfunc et depthfunc
	 */
	void setDataHandler(DataHandler* handler);

	/**
	 * Recupere le pointeur du datahandler
	 */
	DataHandler* getDataHandler();

	/**
	 * Les deux méthodes suivantes sont statiques car freenect a besoin de pointeur de
	 * fonction pour traiter les donnée rgb et de profondeur.
	 */

	/**
	 * traitement des données de profondeur
	 * execute la méthode de traitement de l'interface datahandler
	 * @param freenect_devices* dev kinect en cours d'utilisation
	 * @param void* depthBuffer tableau par lequel arrive les données pour etre traités
	 * @param uint32_t timestamp temps entre deux frames
	 */
	static void depthFunc(freenect_device* dev, void* depthBuffer, uint32_t timestamp);

	/**
	 * traitement des données rgb
	 */

	/**
	 * traitement des données de profondeur
	 * execute la méthode de traitement de l'interface datahandler
	 * @param freenect_devices* dev kinect en cours d'utilisation
	 * @param void* rgb tableau par lequel arrive les données de couleur (un entier par composante de coueleur)
	 * exemple : si l'image capturé est en 640*480, la taille du tableau est de 640*480*3
	 * @param uint32_t timestamp temps entre deux frames
	 */
	static void rgbFunc(freenect_device* dev, void* rgb, uint32_t timestamp);

private:

	/**
	 * Constructeur.
	 * D'abord, on vérifie que l'initialisation du kinect s'est bien passé
	 * puis on defini le seuil du log a fatal
	 * enfin oàn verifie qu'un seul kinect est connecté
	 * et on signal que la camera est prete
	 */
	Kinect();

	virtual ~Kinect();

	/// Pointeur vers l'instance unique
	static Kinect*			myInstance;

	/// Est ce que le kinect est pret a etre utilisé
	bool					myReady;
	/// Continuer la capture ?
	bool					myContinue;

	freenect_context* 		myContext;
	/// Le kinect en cours d'utilisation
	freenect_device* 		myDevice;

	/// Format video du kinect a utiliser
	freenect_video_format	myFormat;

	/// Le thread. On utilise boost car pthread requiert des pointeur de fonction,
	/// or on souheite utiliser des pointeur de methode, ce que permet les thread de boost
	boost::thread			myCaptureThread;

	/// pointeur vers l'apllication qui gerere les données
	DataHandler*			myDataHandler;

	/// Dimension de la resolution
	uint					myResWidth;
	uint					myResHeight;

};

#endif /* KINECT_H_ */
