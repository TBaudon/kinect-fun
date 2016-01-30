
#ifndef BOUTTON_H_
#define BOUTTON_H_

#include <SFML/Graphics.hpp>
#include <string>

class Boutton {

public:

	Boutton(sf::RenderWindow* win, int color, std::string text,int x, int y, int width, int height);

	virtual ~Boutton();

	void update(int x, int y);

	void setColor(int color);

	int getColor();

	int getX();

	int getY();

	int getWidth();

	int getHeight();

	void setPosition(int x, int y);

	bool isPressed();

	int getCounter();

private:

	sf::RenderWindow* 	myWin;
	sf::Sprite			mySprite;
	sf::Text			myDisplayText;

	std::string 		myText;

	int 				myColor;
	int					myWidth, myHeight;
	int					myX, myY;

	int 				myTimeCounter;
	int					myPressTime;

};

#endif /* BOUTTON_H_ */
