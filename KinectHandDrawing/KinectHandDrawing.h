
#ifndef KINECTHAND_H_
#define KINECTHAND_H_

#include "DataHandler.h"
#include "Kinect.h"
#include "Boutton.h"

#include <SFML/Graphics.hpp>

class KinectHandDrawing : public DataHandler{

public:

	/**
	 * Contient les modes de l'application:
	 * Mode dessin et mode menu
	 */
	enum Mode{
		DRAWING,
		MENU
	};

	/**
	 * Constructeur de l'application
	 * On y créer la fenetre a l'aide de sfml
	 * puis on lance la boucle principale et la capture du kinect
	 */
	KinectHandDrawing();

	/**
	 * Destructeur
	 * on y ferme le kinect
	 */
	virtual ~KinectHandDrawing();

	/**
	 * Fonctione de traitement des donnée de profondeur
	 * elle prend en parametre un tableau d'entier codé sur 16 bit
	 * de taille 640*480
	 * Chaque case contient une valeur qui corespond a la profoondeur par rapport au capteur du kinect
	 */
	void depthFunc(uint16_t* depth);

	/**
	 * Fonction de traitement des couleur
	 * elle prend en parametre un tableau d'entier codé sur 8 bit
	 * qui corespond a une composante de couleur
	 * il a donc une taille de 3*640*480
	 */
	void rgbFunc(uint8_t* rgb);

private:

	void updateDist();

	int					myMarginLeft;
	int 				myMarginTop;
	int					myMarginRight;
	int 				myMarginBottom;

	/// Fenetre de rendu
	sf::RenderWindow 	myWin;

	/// Instance du kinect
	Kinect*				myKinect;

	/// Etat courrant de l'application (menu ou dessin)
	Mode				myCurrentMode;

	/// Liste des bouttons pour le menu
	Boutton*			myStartButton, *myQuitButton, **myColorButton;
	Boutton*			myClearButton, *myEraserButton;

	/// Nombre de couleure a afficher dans le menu
	int					myNbColorBout;

	char*				myPoint;

	int 				myWidth, myHeight;

	int					myNear, myFar;
	int					myDistPress, myDistMenu;

	char*				myTexture;
	char*				myCanvas;

	int 				myColor;
	int 				myPaintColor;

	int 				myHandX, myHandY;

	bool				myQuit;

};

#endif /* KINECTCURSEUR_H_ */
