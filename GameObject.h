#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include <SDL.h>
#include <SDL_image.h>
#include "LTexture.h"
#include <iostream>

class GameObject
{
public:
	GameObject();
	~GameObject();				// Deconstructor

	virtual void spawn();
	virtual void spawn(int x, int y);
	void spawn(int x, int y, int vx);
	virtual void movement();

	int getX();					// Get GameObject X coord
	int getY();					// Get GameObject Y coord
	int getVelX();
	int getVelY();
	bool getAlive();

	void setX(int x);			// Set GameObject X coord
	void setY(int y);			// Set GameObject Y coord
	void setVelX(int x);
	void setVelY(int y);
	void setAlive(bool alive);

private:
	// GameObject Variables
	int m_health;				// Value between 0 and 160
	int m_speed;				// Value between 1 and 4
	int m_x;					// GameObject x coord
	int m_y;					// GameObject y coord
	int m_xVel;
	int m_yVel;
	bool m_Alive;				// Is the weapon active on screen
};

#endif
