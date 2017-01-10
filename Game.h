#ifndef GAME_H
#define GAME_H

#include <list>

class Game {
public:
	//bool init();					//Starts up SDL and creates window -- ERROR window won't close
	void update();
	void close();					//Frees media and shuts down SDL
	void spawnEnemyShip();			//added function to create enemy ships at random times and random y coord
	void spawnLaser();
	void spawnNinjaStar();			//added function to create ninja star weapons
	bool playerInput(bool quit);	//Handle input from player
	void renderGameObjects();		//Render the game objects to the screen
	void moveGameObjects();			//Move the game objects on the screen
	void destroyGameObjects();		//Destroy the game objects when finished on the screen
	void playerFlashOnCollide();	//The player texture flashes on collision with other Game Objects
	void engineFX();				//Engine Sound FX for ship
};

#endif