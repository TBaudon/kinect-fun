
#include "KinectHandDrawing.h"

KinectHandDrawing::KinectHandDrawing() {

	myWin.Create(sf::VideoMode(1920, 1080, 32), "KinectHandDrawing",
			sf::Style::Fullscreen);
	myWin.SetFramerateLimit(60);

	myWidth = myWin.GetWidth();
	myHeight = myWin.GetHeight();

	myHandX = 0;
	myHandY = 0;

	myQuit = false;

	/**
	 * Definition des Bouttons
	 */

	myStartButton = new Boutton(&myWin, 0x009900ff, "Dessiner", 0, 0,
			myWidth * 0.1f, myHeight * 0.1f);

	int quitX = myWidth - myWidth * 0.1f;

	myQuitButton = new Boutton(&myWin, 0xcc0000ff, "Quitter", quitX, 0,
			myWidth * 0.1f, myHeight * 0.1f);

	myClearButton = new Boutton(&myWin, 0x0000ccff, "Clear", 6 * (myWidth / 10),
			0, myWidth * 0.1f, myHeight * 0.1f);

	myEraserButton = new Boutton(&myWin, 0xff6666ff, "Gomme",
			3 * (myWidth / 10), 0, myWidth * 0.1f, myHeight * 0.1f);

	myNbColorBout = 16;

	myColorButton = new Boutton*[myNbColorBout];

	int boutx = 0;
	int bouty = myHeight - myHeight / 2;

	int r = 0;
	int g = 0;
	int b = 0;

	int pas = 0xff/2;

	for (int i = 0; i < myNbColorBout; ++i) {
		if(r > 0xff)
			r = 0;
		if(g > 0xff)
			g = 0;
		if(b > 0xff)
			b = 0;
		int couleur = r << 8 * 3 | g << 8 * 2 | b << 8 | 0xff;
		r += pas;
		if (r >= 0xff)
			g += pas;
		if (g >= 0xff)
			b += pas;
		if (b >= 0xff)
			b = 0;

		myColorButton[i] = new Boutton(&myWin, couleur, "",
				i * myWidth / myNbColorBout, bouty, myWidth / myNbColorBout,
				myHeight * 0.1f);
	}

	sf::Event ev;

	myCurrentMode = MENU;

	myColor = 0x99999966;
	myPaintColor = 0x000000ff;

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

	myFar = 1650;
	myNear = 500;

	myMarginLeft = 0;
	myMarginRight = 0;
	myMarginTop = 0;
	myMarginBottom = 0;

	myCanvas = new char[640 * 480 * 4];
	for (int i = 0; i < 640 * 480; ++i) {
		myCanvas[i * 4] = 0x00;
		myCanvas[i * 4 + 1] = 0x00;
		myCanvas[i * 4 + 2] = 0x00;
		myCanvas[i * 4 + 3] = 0x00;
	}

	sf::Texture canvasTex;
	canvasTex.Create(640, 480);
	canvasTex.Update((sf::Uint8*) myCanvas);

	sf::Sprite canvasSprite(canvasTex);
	canvasSprite.Scale((float) myWidth / 640.0f, (float) myHeight / 480.0f);

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

		if (myQuit)
			myWin.Close();

		tex.Update((sf::Uint8*) myTexture);
		canvasTex.Update((sf::Uint8*) myCanvas);

		myWin.Clear(sf::Color(255, 255, 255));

		myWin.Draw(canvasSprite);

		/**
		 * Dessin du menu
		 */

		if (myCurrentMode == MENU) {
			myStartButton->update(myHandX, myHandY);
			myQuitButton->update(myHandX, myHandY);
			myClearButton->update(myHandX, myHandY);
			myEraserButton->update(myHandX, myHandY);
			for (int i = 0; i < myNbColorBout; ++i) {
				myColorButton[i]->update(myHandX, myHandY);
			}
		}

		myWin.Draw(affichage);

		myWin.Display();

	}

}

KinectHandDrawing::~KinectHandDrawing() {
	myKinect->close();
}

void KinectHandDrawing::updateDist() {

	int ecart = myFar - myNear;

	myDistPress = myNear + ecart / 5;
	myDistMenu = myNear + (ecart / 5) * 4;

}

void KinectHandDrawing::depthFunc(uint16_t* depth) {

	int distMoyenne = 0;
	int diviseur = 0;
	int min = 0xffff;

	for (int i = 0; i < 480; ++i) {
		for (int j = 0; j < 640; ++j) {

			int imgIndex = (640 * i + 640 - j) * 4;
			int currentDepth = depth[640 * i + j];

			char R = 0xFF & (myColor >> 8 * 3);
			char V = 0xFF & (myColor >> 8 * 2);
			char B = 0xFF & (myColor >> 8 * 1);
			char A = 0xFF & (myColor >> 8 * 0);

			char PR = 0xFF & (myPaintColor >> 8 * 3);
			char PV = 0xFF & (myPaintColor >> 8 * 2);
			char PB = 0xFF & (myPaintColor >> 8 * 1);
			char PA = 0xFF & (myPaintColor >> 8 * 0);

			if (currentDepth > myNear && currentDepth < myFar) {
				distMoyenne += currentDepth;
				diviseur++;

				/**
				 * Dessine la silouette dans la texture
				 */
				myTexture[imgIndex] = R;
				myTexture[imgIndex + 1] = V;
				myTexture[imgIndex + 2] = B;
				myTexture[imgIndex + 3] = A;

				/**
				 * Paint dans la texture
				 */
				if (currentDepth < myDistPress && myCurrentMode == DRAWING) {
					myCanvas[imgIndex] = PR;
					myCanvas[imgIndex + 1] = PV;
					myCanvas[imgIndex + 2] = PB;
					myCanvas[imgIndex + 3] = PA;
				}

			} else {
				myTexture[imgIndex] = 0;
				myTexture[imgIndex + 1] = 0;
				myTexture[imgIndex + 2] = 0;
				myTexture[imgIndex + 3] = 0;
			}

			/**
			 * Position de la main aproximative pour le menu
			 */

			if (currentDepth < min && currentDepth > myNear
					&& currentDepth < myFar) {
				min = currentDepth;
				myHandX = (640 - j) * myWidth / 640;
				myHandY = i * myHeight / 480;
			}

		}
	}

	if (diviseur != 0 && myCurrentMode != MENU)
		distMoyenne /= diviseur;
	else
		distMoyenne = myDistMenu;

	/**
	 * Gere les different etat de l'application;
	 * Dessin, menu
	 */

	if (distMoyenne >= myDistMenu)
		myCurrentMode = MENU;
	else
		myCurrentMode = DRAWING;

	/**
	 * Appui sur les bouttons en mode menu
	 */

	if (myCurrentMode == MENU) {
		if (myStartButton->isPressed())
			myCurrentMode = DRAWING;
		else if (myQuitButton->isPressed())
			myQuit = true;
		else if(myClearButton->isPressed())
			for(int i =0; i < 640*480*4;++i)
				myCanvas[i]  = 0;
		else if(myEraserButton->isPressed()){
			myPaintColor = 0;
			myCurrentMode = DRAWING;
		}
		for (int i = 0; i < myNbColorBout; ++i) {
			if (myColorButton[i]->isPressed()) {
				myPaintColor = myColorButton[i]->getColor();
				myCurrentMode = DRAWING;
			}
		}
	}

}

void KinectHandDrawing::rgbFunc(uint8_t * rgb) {

}

