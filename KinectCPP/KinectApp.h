/*
 * KinectApp.h
 *
 *  Created on: 15 févr. 2012
 *      Author: thomas
 */

#ifndef KINECTAPP_H_
#define KINECTAPP_H_

#include "DataHandler.h"
#include "Kinect.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

class KinectApp : public DataHandler{

public:

	KinectApp();

	virtual ~KinectApp();

	void depthFunc(uint16_t* depth);

	void rgbFunc(uint8_t* rgb);

private:

	Kinect* 				myKinect;

	char*					myTexture;

	bool*					myPoint;

	bool					myDepthOk;
	bool					myRGBOk;

	int 					myLimitFar, myLimitNear;

	int						myCursorX, myCursorY;

	std::vector<sf::Sprite*> myStars;

};

#endif /* KINECTAPP_H_ */
