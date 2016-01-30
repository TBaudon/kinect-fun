/*
 * KinectCurseur.h
 *
 *  Created on: 15 mars 2012
 *      Author: Thomas BAUDON
 */

#ifndef KINECTCURSEUR_H_
#define KINECTCURSEUR_H_

#include "DataHandler.h"
#include "Kinect.h"

#include <SFML/Graphics.hpp>

class KinectCurseur : public DataHandler{

public:

	KinectCurseur();

	virtual ~KinectCurseur();

	void depthFunc(uint16_t* depth);

	void rgbFunc(uint8_t* rgb);

	void decalageDroite(int* tab, int taille, int pos);

private:

	void updateDist();

	int					myMarginLeft;
	int 				myMarginTop;
	int					myMarginRight;
	int 				myMarginBottom;

	sf::RenderWindow 	myWin;

	Kinect*				myKinect;

	char*				myPoint;

	int 				myWidth, myHeight;

	int					myNear, myFar;
	int					myDistPress, myDistOut, myDistMenu;

	int					myCurX, myCurY;

	char*				myTexture;
	char*				myCanvas;

};

#endif /* KINECTCURSEUR_H_ */
