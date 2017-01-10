#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"

class EnemyShip : public GameObject {
public:
	EnemyShip();							// Initializes the variables
	//~EnemyShip();
	
	// The dimensions of the Enemy (dimensions of sprite image)
	static const int ENEMY_WIDTH = 100;
	static const int ENEMY_HEIGHT = 47;

	static const int ENEMY_VEL = 20;		// Maximum axis velocity of the Enemy

	virtual void movement();
	void render();							// Shows the Enemy on the screen

private:
	//int mEnPosX, mEnPosY;					// The X and Y offsets of the Enemy	
	int mEnVelX, mEnVelY;					// The velocity of the Enemy
	int mEnAlive;

};

#endif
