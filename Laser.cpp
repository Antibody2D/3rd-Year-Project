
/* 2017/01/09:
Moved functionality common to game objects to GameObjects class reducing the code
*/

/*
* Laser.cpp is where all the implementations of the function definitions in Laser.h reside.
*/

#include "Laser.h"
#include "Ship.h"

// Laser Constructor
Laser::Laser()
{
	std::cout << "Laser constuctor called.\n";
}

// Laser Destructor
Laser::~Laser()
{
	std::cout << "Laser destructor called.\n";
}

void Laser::movement() {
	GameObject::movement();

	// destroy laser beam once it is offscreen
	if (getX() > SCREEN_WIDTH) setAlive(false);
	else setAlive(true);
}

void Laser::spawn(int x, int y) {
	setX(x + 65);
	setY(y + 30);
	setVelX(LASER_VEL);
	setVelY(0);
}