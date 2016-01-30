
#include "Boutton.h"

Boutton::Boutton(sf::RenderWindow* win, int color, std::string text, int x,
		int y, int width, int height) {

	myWin = win;

	myWidth = width;
	myHeight = height;

	myTimeCounter = 0;

	sf::Texture texture;
	texture.Create(myWidth, myHeight);

	myText = text;
	if(myText != ""){
		myDisplayText = sf::Text(myText);
		int w = myDisplayText.GetGlobalBounds().Width;
		int h = myDisplayText.GetGlobalBounds().Height;

		myDisplayText.SetPosition(x + width/2 - w/2, y +height/2 - h/2 );
	}

	myPressTime = 10;

	mySprite = sf::Sprite(texture);

	setColor(color);
	setPosition(x, y);

}

Boutton::~Boutton() {
	// TODO Auto-generated destructor stub
}

void Boutton::update(int x, int y) {
	if (x > myX && x < myX + myWidth && y > myY && y < myY + myHeight){
		++myTimeCounter;

		char R = 0xFF & (myColor >> 8 * 3);
		char V = 0xFF & (myColor >> 8 * 2);
		char B = 0xFF & (myColor >> 8 * 1);
		char A = 0xFF & (myColor >> 8 * 0);

		mySprite.SetColor(sf::Color(R,V,B,A - myTimeCounter*2));
	}
	else{
		myTimeCounter = 0;
		setColor(myColor);
	}

	myWin->Draw(mySprite);
	if(myText != "")
		myWin->Draw(myDisplayText);
}

void Boutton::setColor(int color) {
	myColor = color;

	char R = 0xFF & (color >> 8 * 3);
	char V = 0xFF & (color >> 8 * 2);
	char B = 0xFF & (color >> 8 * 1);
	char A = 0xFF & (color >> 8 * 0);

	mySprite.SetColor(sf::Color(R, V, B, A));
}

int Boutton::getX() {
	return myX;
}

int Boutton::getY() {
	return myY;
}

int Boutton::getWidth() {
	return myWidth;
}

int Boutton::getHeight() {
	return myHeight;
}

void Boutton::setPosition(int x, int y) {
	myX = x ;
	myY = y ;
	mySprite.SetPosition(myX, myY);
}

int Boutton::getCounter() {
	return myTimeCounter;
}

bool Boutton::isPressed() {
	bool rep = myTimeCounter > myPressTime;
	if(rep)
		myTimeCounter = 0;
	return rep;
}

int Boutton::getColor(){
	return myColor;
}

