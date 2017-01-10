#include "Game.h"
#include "EnemyShip.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

EnemyShip::EnemyShip() {
	setX(0);
	setY(0);

	setVelX(0);
	setVelY(0);
}

void EnemyShip::movement() {
	GameObject::movement();

	// destroy enemy ship once it is offscreen
	if (getX() < -100) setAlive(false);
	else setAlive(true);
}
