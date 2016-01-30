/*
 * Kinect.cpp
 *
 *  Created on: 14 févr. 2012
 *      Author: thomas
 */

#include "Kinect.h"

#include <iostream>

#include "DataHandler.h"

using namespace std;

Kinect* Kinect::myInstance = 0;

Kinect* Kinect::open(){
	if(!myInstance)
		myInstance = new Kinect();
	return myInstance;
}

Kinect::Kinect() {

	myReady = false;

	cout << "Initialisation du pilote: ";
	flush(cout);

	// Vérification de l'initialisation
	if(freenect_init(&myContext, 0) < 0){
		cout << "Erreur!" << endl;
		return;
	}

	cout << "OK!" << endl;

	// Definition du seuil du log
	freenect_set_log_level(myContext, FREENECT_LOG_FATAL);

	// On signal quelle partie du kinect on va utiliser (ici, les moteur et la camera
	freenect_select_subdevices(myContext, (freenect_device_flags) (FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

	//On detecte combien de kinects sont branchés
	char deviceNb = freenect_num_devices(myContext);

	if(deviceNb != 1){
		cout << "Erreur! Nombre de kinect détecté: " << deviceNb << endl;
		return;
	}

	cout << "Initialisation du Kinect: ";
	flush(cout);

	// Ouverture du peripherique
	if(freenect_open_device(myContext, &myDevice, 0) < 0){
		cout << "Erreur!" << endl;
		return;
	}

	cout << "OK!" << endl;

	// Ici on met la led clignotante jaune pour signaler que la camera est prete
	freenect_set_led(myDevice, LED_YELLOW);

	myReady = true;

}

Kinect::~Kinect() {

}

void Kinect::close(){
	if(myContinue)
		stopCapture();
	delete myInstance;
}

void Kinect::startCapture(){
	myContinue = true;
	myCaptureThread = boost::thread(&Kinect::captureThread, this);
}

void Kinect::stopCapture(){
	myContinue = false;
	myCaptureThread.join();
}

void Kinect::captureThread(){
	cout << "Capture commencée." << endl;

	freenect_set_led(myDevice, LED_GREEN);

	// Ici on dit quelle methode utiliser pour traiter les donnée de profondeur
	freenect_set_depth_callback(myDevice, &Kinect::depthFunc);

	// Ici on defini la resolution et le mode d'alignement par rapport au rgb
	freenect_set_depth_mode(myDevice, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_MM));
	freenect_start_depth(myDevice);

	myFormat = FREENECT_VIDEO_RGB;

	// Pareil pour les rgb
	freenect_set_video_callback(myDevice, &Kinect::rgbFunc);
	freenect_set_video_mode(myDevice, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, myFormat));
	freenect_start_video(myDevice);

	// Temps que continue est a vrai on traite els donnée recu
	// (process_event esecute le callback rgb et depth
	while(myContinue && freenect_process_events(myContext) >= 0){}

	cout << "Fin de capture, libération du Kinect." << endl;

	// A partir d'ici on liberte les ressource
	// du kinect

	freenect_set_led(myDevice, LED_BLINK_GREEN);

	freenect_close_device(myDevice);
	freenect_shutdown(myContext);
}

void Kinect::setDataHandler(DataHandler* handler){
	myDataHandler = handler;
}

DataHandler* Kinect::getDataHandler(){
	return myDataHandler;
}

void Kinect::depthFunc(freenect_device* dev, void* depthBuffer, uint32_t timestamp){

	DataHandler* handler = Kinect::open()->getDataHandler();
	if(handler)
		handler->depthFunc((uint16_t*)depthBuffer);

}

void Kinect::rgbFunc(freenect_device* dev, void* rgb, uint32_t timestamp){

	DataHandler* handler = Kinect::open()->getDataHandler();
	if(handler)
		handler->rgbFunc((uint8_t*)rgb);

}



