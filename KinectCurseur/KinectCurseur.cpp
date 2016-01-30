/*
 * KinectCurseur.cpp
 *
 *  Created on: 15 mars 2012
 *      Author: Thomas BAUDON
 */

#include "KinectCurseur.h"

KinectCurseur::KinectCurseur() {

	myWin.Create(sf::VideoMode(1920, 1080, 32), "KinectCurseur",
			sf::Style::Fullscreen);
	myWin.SetFramerateLimit(60);

	myWidth = myWin.GetWidth();
	myHeight = myWin.GetHeight();

	sf::Event ev;

	myKinect = Kinect::open();
	myKinect->setDataHandler(this);

	myTexture = new char[640 * 480 * 4];

	myPoint = new char[640 * 480];
	for (int i = 0; i < 640 * 480; ++i)
		myPoint[i] = 0;

	sf::Texture tex;
	tex.Create(640, 480);

	sf::Sprite affichage(tex);
	affichage.SetPosition(0, 0);
	affichage.Scale((float) myWidth / tex.GetWidth(),
			(float) myHeight / tex.GetHeight());

	myFar = 1700;
	myNear = 500;

	myMarginLeft = 0;
	myMarginRight = 0;
	myMarginTop = 0;
	myMarginBottom = 0;

	myCurX = -10;
	myCurY = -10;

	char* cursorTexture = new char[10 * 10 * 4];

	for (int i = 0; i < 10 * 10; ++i) {
		cursorTexture[i * 4] = 0x00;
		cursorTexture[i * 4 + 1] = 0x00;
		cursorTexture[i * 4 + 2] = 0x00;
		cursorTexture[i * 4 + 3] = 0xff;
	}

	sf::Texture texcur;
	texcur.Create(10, 10);
	texcur.Update((sf::Uint8*) cursorTexture);

	sf::Sprite cursor(texcur);

	myCanvas = new char[1920 * 1080 * 4];
	for (int i = 0; i < 1920 * 1080; ++i) {
		myCanvas[i * 4] = 0x00;
		myCanvas[i * 4 + 1] = 0x00;
		myCanvas[i * 4 + 2] = 0x00;
		myCanvas[i * 4 + 3] = 0x00;
	}

	sf::Texture canvasTex;
	canvasTex.Create(1920, 1080);
	canvasTex.Update((sf::Uint8*) myCanvas);

	sf::Sprite canvasSprite(canvasTex);

	updateDist();

	myKinect->startCapture();

	while (myWin.IsOpen()) {

		while (myWin.PollEvent(ev)) {
			if (ev.Type == sf::Event::Closed)
				myWin.Close();
			if (ev.Type == sf::Event::KeyPressed) {
				switch (ev.Key.Code) {
				case sf::Keyboard::Add:
					myFar += 50;
					break;
				case sf::Keyboard::Subtract:
					myFar -= 50;
					break;
				case sf::Keyboard::Up:
					myNear += 50;
					break;
				case sf::Keyboard::Down:
					myNear -= 50;
					break;
				case sf::Keyboard::Escape:
					myWin.Close();
					break;
				default:
					break;
				}
			}
		}

		tex.Update((sf::Uint8*) myTexture);
		canvasTex.Update((sf::Uint8*) myCanvas);

		cursor.SetPosition(myCurX, myCurY);

		myWin.Clear(sf::Color(255, 255, 255));

		myWin.Draw(affichage);
		myWin.Draw(canvasSprite);
		myWin.Draw(cursor);

		myWin.Display();

	}

}

KinectCurseur::~KinectCurseur() {
	myKinect->close();
}

void KinectCurseur::updateDist() {
	int ecart = myFar - myNear;

	myDistPress = myNear + ecart / 4;
	myDistOut = myNear + (ecart / 4) * 2;
	myDistMenu = myNear + (ecart / 4) * 3;

}

void KinectCurseur::decalageDroite(int* tab, int taille, int pos) {
	if (pos < taille - 1) {
		int temp = tab[pos];
		decalageDroite(tab, taille, pos + 1);
		tab[pos + 1] = temp;
	}
}

void KinectCurseur::depthFunc(uint16_t* depth) {

	int tempX = -10;
	int tempY = -10;

	int lesX[20];
	int lesY[20];

	int mini[20];
	for (int i = 0; i < 20; ++i)
		mini[i] = 0xffff;

	for (int i = myMarginTop; i < 480 - myMarginBottom; ++i)
		for (int j = myMarginLeft; j < 640 - myMarginRight; ++j) {

			int index = i * 640 + 640 - j;
			int currentDepth = depth[i * 640 + j];

			int dist = myFar - myDistPress;

			if (currentDepth >= myNear && currentDepth <= myFar) {
				char alpha = 255
						- (float) (currentDepth - myDistPress) / (float) dist
								* 255;
				if (currentDepth <= myDistPress) {
					alpha = 255;
					/*myCanvas[index * 4] = 0x44;
					 myCanvas[index * 4 + 1] = 0x22;
					 myCanvas[index * 4 + 2] = 0x00;
					 myCanvas[index * 4 + 3] = 0xff;*/
				}

				myPoint[index] = alpha;
			} else
				myPoint[index] = 0;

			if (currentDepth > myNear)
				for (int k = 0; k < 20; k++) {
					if (currentDepth < mini[k]) {
						decalageDroite(mini, 20, k);
						mini[k] = currentDepth;
						decalageDroite(lesX, 20, k);
						lesX[k] = j;
						decalageDroite(lesY, 20, k);
						lesY[k] = i;
					}
				}

			int moyX;

			for (int k = 0; k < 20; ++k)
				moyX += lesX[k];

			int moyY;

			for (int k = 0; k < 20; ++k)
				moyY += lesY[k];

			moyX /= 20;
			moyY /= 20;

			tempX = 640 - moyX;
			tempY = moyY;

			tempX *= (float) myWidth / 640.0f;
			tempY *= (float) myHeight / 480.0f;

		}

	if (myCurX != -10 && myCurY != -10) {

		int distx = tempX - myCurX;
		int disty = tempY - myCurY;

		double dist = sqrt(distx * distx + disty * disty);

		if (dist < 200) {
			myCurX = tempX;
			myCurY = tempY;
		}

	} else {
		myCurX = tempX;
		myCurY = tempY;
	}

}

void KinectCurseur::rgbFunc(uint8_t * rgb) {

	for (int i = myMarginTop; i < 480 - myMarginBottom; ++i)
		for (int j = myMarginLeft; j < 640 - myMarginRight; ++j) {

			int a = i * 640 + 640 - j;
			int b = i * 640 + j;

			if (myPoint[b] != 0) {
				myTexture[b * 4] = rgb[a * 3];
				myTexture[b * 4 + 1] = rgb[a * 3 + 1];
				myTexture[b * 4 + 2] = rgb[a * 3 + 2];
				myTexture[b * 4 + 3] = myPoint[b];
			} else {
				myTexture[b * 4] = 0;
				myTexture[b * 4 + 1] = 0;
				myTexture[b * 4 + 2] = 0;
				myTexture[b * 4 + 3] = 0;
			}

		}
}

