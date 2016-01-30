/*
 * KinectApp.cpp
 *
 *  Created on: 15 févr. 2012
 *      Author: thomas
 */

#include "KinectApp.h"
#include <unistd.h>
#include <time.h>

#define NB_STAR 500

KinectApp::KinectApp() {

	srand(time(0));

	/**
	 * Initialisation Kinect
	 */
	myKinect = Kinect::open();
	myKinect->setDataHandler(this);

	myDepthOk = false;
	myRGBOk = false;

	myPoint = new bool[640*480];
	for(int i = 0; i < 640*480; ++i)
		myPoint[i] = false;

	myTexture = new char[640*480*4];

	sf::Texture texStar;
	texStar.LoadFromFile("Star_max-b.png");

	for(int i = 0; i < NB_STAR; ++i){
		sf::Sprite* etoile = new sf::Sprite(texStar);
		etoile->SetScale(0.05,0.05);
		etoile->SetOrigin(0.5,0.5);
		etoile->SetPosition(rand()%640, rand()%480);
		myStars.push_back(etoile);
	}

	sf::RenderWindow win(sf::VideoMode(640,480,32), "KinectCPP");
	win.SetFramerateLimit(60);

	sf::Event event;

	sf::Texture tex;
	tex.Create(640,480);

	sf::Texture space;
	space.LoadFromFile("espace_02.jpg");

	sf::Sprite fond(space);

	sf::Sprite sprite(tex);
	sprite.SetScale(-1,1);
	sprite.SetPosition(640,0);

	myLimitFar = 2200;
	myLimitNear = 1000;

	/**
	 * Debut capture, a lancer avant boucle de jeu.
	 */
	myKinect->startCapture();

	while(win.IsOpen()){
		while(win.PollEvent(event)){
			if(event.Type == sf::Event::Closed)
				win.Close();
			if(event.Type == sf::Event::KeyPressed){
				switch(event.Key.Code){
				case sf::Keyboard::Add:
					myLimitFar += 50;
					break;
				case sf::Keyboard::Subtract:
					myLimitFar -= 50;
					break;
				case sf::Keyboard::Up:
					myLimitNear += 50;
					break;
				case sf::Keyboard::Down:
					myLimitNear -= 50;
					break;
				case sf::Keyboard::Escape:
					win.Close();
					break;
				default:
					break;
				}
			}

		}

		if(myRGBOk)
			tex.Update((sf::Uint8*)myTexture);



		win.Clear();

		win.Draw(fond);
		win.Draw(sprite);

		for(int i = 0; i < NB_STAR; ++i){
			int x = myStars[i]->GetPosition().x;
			int y = myStars[i]->GetPosition().y;
			if(!myPoint[y * 640 + 640-x])
				myStars[i]->Move(0,2);
			if(myStars[i]->GetPosition().y > 480)
				myStars[i]->SetPosition(rand()%640, 0);
			win.Draw(*myStars[i]);
		}

		win.Display();
	}

}

KinectApp::~KinectApp() {

	myKinect->close();

}

void KinectApp::depthFunc(uint16_t* depth){

	myDepthOk = false;

	for(int i = 0; i < 480; ++i)
		for(int j = 0; j < 640; ++j)
			if(depth[i*640+j] <= myLimitFar && depth[i*640+j] >= myLimitNear)
				myPoint[i*640+j] = true;
			else
				myPoint[i*640+j] = false;

	myDepthOk = true;

}

void KinectApp::rgbFunc(uint8_t* rgb){

	myRGBOk = false;

	for(int i = 0; i < 640*480; ++i){

		if(myPoint[i]){
			myTexture[i*4] = rgb[i*3];
			myTexture[i*4 +1] = rgb[i*3 +1];
			myTexture[i*4 +2] = rgb[i*3 +2];
			myTexture[i*4 +3] = 255;
		}else{
			myTexture[i*4] = 255;
			myTexture[i*4 +1] = 255;
			myTexture[i*4 +2] = 255;
			myTexture[i*4 +3] = 0;
		}
	}

	myRGBOk = true;

}

