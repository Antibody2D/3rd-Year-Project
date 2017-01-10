#include "Game.h"

int main()
{
	Game game;				// Create a game object

	game.update();

	game.close();			// Free resources and close SDL

	return 0;
}