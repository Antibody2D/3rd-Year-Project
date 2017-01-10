// 09/01 Edited background to be 800 x 600 instead of 600 * 480

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>			//SOUND - library we use to make audio playing easier
#include "Game.h"
#include "LTexture.h"
#include "Ship.h"
#include "EnemyShip.h"
#include "Laser.h"
#include "NinjaStar.h"

//The music that will be played
Mix_Music *gMusic = NULL;		// Mix_Music: Data type for music

//The sound effects that will be used (pointers)
Mix_Chunk *gNinjaFX = NULL;		//SOUND - Mix_Chunk: Data type for short sounds
Mix_Chunk *gLaserFX = NULL;		//SOUND - Mix_Chunk: Data type for short sounds
Mix_Chunk *gEngineFX = NULL;	//SOUND - Mix_Chunk: Data type for short sounds

bool init();					// Starts up SDL and creates window
bool loadMedia();				// Loads media//void close();

SDL_Window* gWindow = NULL;		// The window we'll be rendering to
SDL_Renderer* gRenderer = NULL;	// The window renderer
Uint8 a = 0;					// Modulation component

int degrees = 0;
int alphaUp = 5, alphaDown = 5;
int scrollingOffset = 0;

//Scene textures
LTexture gBGTexture;			// Background
LTexture gShipTexture;			// Player ship
LTexture gEnemyShipTexture;		// Enemy ship
LTexture gLaserTexture;			// Texture for Laser weapon
LTexture gNinjaStar;			// Texture for Ninja Star weapon
//LTexture gGO;

//Move ship object outside of main so spawn Laser funtion can use it
Ship player;					// Declare a ship object that will be moving around on the screen
EnemyShip* enemy1 = new EnemyShip();
EnemyShip* enemy2 = new EnemyShip();

std::list<EnemyShip*> listOfEnemyShips;			//List to store laser objects
std::list<EnemyShip*>::const_iterator iterES;	//Make them read only
//Created list and iterator for laser objects
std::list<Laser*> listOfLaserObjects;			// List to store laser objects
std::list<Laser*>::const_iterator iter;			// Make them read only

std::list<NinjaStar*> listOfNinjaStarObjects;	//List to store Ninja Star objects											
std::list<NinjaStar*>::const_iterator iterNS;	//Create global iterators to cycle through laser objects - Make them read only


/*gRenderer*/
bool LTexture::loadFromFile(std::string path) {	
	free();													// Get rid of preexisting texture
		
	SDL_Texture* newTexture = NULL;							// The final texture
	
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());	// Load image at specified path
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	} else {		
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));	// Color key image
				
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);						// Create texture from surface pixels
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		} else {
			// Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);	// Get rid of old loaded surface
	}

	// Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };	// Set rendering space and render to screen

	// Set clip rendering dimensions
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);	// Render to screen
}

bool init() {
	bool success = true;					// Initialization flag

	//player.spawn(10, SCREEN_HEIGHT / 2);
	(*enemy1).spawn(850,200, -5);
	(*enemy2).spawn(800, 400, -5);				// Spawn x, y, and x velocity
	listOfEnemyShips.push_back(enemy1);
	listOfEnemyShips.push_back(enemy2);

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {	//SOUND - Neet do initialise audio with video
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		// Create window
		gWindow = SDL_CreateWindow("Journey To The Center Of My Headache", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);	/* Window name */
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// Create vsynced renderer for window
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);										// Initialize renderer color

				int imgFlags = IMG_INIT_PNG;																	// Initialize PNG loading
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//call Mix_oopenAudio to Initialize SDL_mixer 
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)	// sound frequencey, sample format, hardware channels, sample size
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());	// report errors with SDL_mixer
					success = false;
				}
				//Mix_PlayMusic(gMusic, -1);
			}
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;			// Loading success flag

	// Load Ship texture
	if (!gShipTexture.loadFromFile("PlayerShip1.png")) {
		printf("Failed to load Player texture!\n");
		success = false;
	}

	// Load Enemy Ship texture
	if (!gEnemyShipTexture.loadFromFile("EnemyShip.png")) {
		printf("Failed to load Enemy texture!\n");
		success = false;
	}

	// Load background texture
	if (!gBGTexture.loadFromFile("bg800.png")) {			// 09/01 Edited background to be 800 x 600 instead of 600 * 480
		printf("Failed to load background texture!\n");
		success = false;
	}

	// Load Laser texture
	if (!gLaserTexture.loadFromFile("LaserBeam.png")) {
		printf("Failed to load Laser texture!\n");
		success = false;
	}

	// Load Ninja Star texture
	if (!gNinjaStar.loadFromFile("ninjastar1.png")) {
		printf("Failed to load Ninja Star texture!\n");
		success = false;
	}

	//Load music
	gMusic = Mix_LoadMUS("SleepNow2.wav");	// Load music
	if (gMusic == NULL)
	{
		printf("Failed to load rage music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load sound effects
	gNinjaFX = Mix_LoadWAV("Swoosh2.wav");	// Load sound effects
	if (gNinjaFX == NULL) {
		printf("Failed to load ninja star sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	gLaserFX = Mix_LoadWAV("Laser3.wav");	// Load sound effects
	if (gNinjaFX == NULL) {
		printf("Failed to load laser beam sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	gEngineFX = Mix_LoadWAV("Engine1.wav");	// Load sound effects
	if (gNinjaFX == NULL) {
		printf("Failed to load laser beam sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	Mix_PlayMusic(gMusic, -1);

	return success;
}

void Game::close() {
	// Free loaded images
	gShipTexture.free();
	gEnemyShipTexture.free();
	gBGTexture.free();
	gLaserTexture.free();
	gNinjaStar.free();

	//Free the sound effects
	Mix_FreeChunk(gNinjaFX);	// Free a sound effect
	Mix_FreeChunk(gLaserFX);	// Free a sound effect
	Mix_FreeChunk(gEngineFX);	// Free a sound effect

	//Free the music
	Mix_FreeMusic(gMusic);	// Free music
	gMusic = NULL;

	// Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Game::update(){
	// Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		// Load media
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			bool quit = false;							// Main loop flag

			// While application is running
			while (!quit) {
				// Handle events on queue
				playerFlashOnCollide();					//The player texture flashes on collision with other Game Objects

				quit = playerInput(quit);				//Handle input from player

				renderGameObjects();					//Render the game objects to the screen

				moveGameObjects();						//Move the game objects on the screen

  				SDL_RenderPresent(gRenderer);			// Update screen

				destroyGameObjects();					//Destroy the game objects when finished on the screen

				if (listOfEnemyShips.size() == 0) {
					spawnEnemyShip();
					engineFX();							//START ENGINE FX WHEN ENEMY SHIP SPAWNED
				}
			}
		}
	}
}

bool Game::playerInput(bool quit = false) {
	SDL_Event e;								// Event handler

	while (SDL_PollEvent(&e) != 0) {
		// User requests quit	EXIT - CLOSE WINDOW
		if (e.type == SDL_QUIT) {
			quit = true;
		}

		player.handleEvent(e);				// Handle input for the ship
	}
	return quit;
}

void Game::renderGameObjects() {

	degrees += 5;								// Number of degrees to rotate spinning objects
	degrees %= 360;	

	// Scroll background
	--scrollingOffset;
	if (scrollingOffset < -gBGTexture.getWidth()) {
		scrollingOffset = 0;				// update the scrolling background
	}
	// Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	// Render background
	gBGTexture.render(scrollingOffset, 0);
	gBGTexture.render(scrollingOffset + gBGTexture.getWidth(), 0);

	// Cycle through list of Enemy ships and render to screen
	for (iterES = listOfEnemyShips.begin(); iterES != listOfEnemyShips.end();) {
		(*iterES++)->render();	// Render the laser
	}

	// Cycle through list of laser objects and render them to screen
	for (iter = listOfLaserObjects.begin(); iter != listOfLaserObjects.end();) {
		(*iter++)->render();	// Render the laser
	}
	for (iterNS = listOfNinjaStarObjects.begin(); iterNS != listOfNinjaStarObjects.end();) {
		(*iterNS++)->render();	// Render the laser
	}

	/* Set the Alpha value for Enemy */
	//gEnemyShipTexture.setAlpha(a);			// will flash for all enemies NO GOOD
	gShipTexture.setAlpha(a);
	player.render();							// render the ship over the background
}

void Game::moveGameObjects() {
	player.movement();						// Update ship movement	

	// Cycle through list of Enemy ships and move them
	for (iterES = listOfEnemyShips.begin(); iterES != listOfEnemyShips.end();) {
		(*iterES++)->movement();	// Render the laser
	}

	// Cycle through list of laser objects and move them
	for (iter = listOfLaserObjects.begin(); iter != listOfLaserObjects.end();) {
		(*iter++)->movement();					// Move the laser
	}
	for (iterNS = listOfNinjaStarObjects.begin(); iterNS != listOfNinjaStarObjects.end();) {
		(*iterNS++)->movement();	// Render the laser
	}
}
// Destroy Game Objects
void Game::destroyGameObjects() {
	for (iterES = listOfEnemyShips.begin(); iterES != listOfEnemyShips.end();) {
		if (!(*iterES)->getAlive()) {
			iterES = listOfEnemyShips.erase(iterES);
			std::cout << "destroy enemy ship" << std::endl;
		}
		else {
			iterES++;
		}
	}
	for (iter = listOfLaserObjects.begin(); iter != listOfLaserObjects.end();) {
		if (!(*iter)->getAlive()) {
			iter = listOfLaserObjects.erase(iter);
			std::cout << "destroy laser" << std::endl;
		}
		else {
			iter++;
		}
	}
	for (iterNS = listOfNinjaStarObjects.begin(); iterNS != listOfNinjaStarObjects.end();) {
		if (!(*iterNS)->getAlive()) {
			iterNS = listOfNinjaStarObjects.erase(iterNS);
			std::cout << "destroy ninja star" << std::endl;
		}
		else {
			iterNS++;
		}
	}
}

void Game::playerFlashOnCollide() {
	if (alphaUp < 255) {
		alphaUp += 10;

		if (a > 255) a = 255;
		else a = alphaUp;

		if (alphaUp >= 255) alphaDown = 255;
	}

	if (alphaDown > 5) {
		alphaDown -= 10;

		if (a < 5) a = 5;
		else a = alphaDown;

		if (alphaDown <= 5) alphaUp = 5;
	}
}

// List of enemy ships to spawn at random times and positions
void Game::spawnEnemyShip() {
	int y = SCREEN_HEIGHT / 2;
	EnemyShip* p_Enemy = new EnemyShip();
	p_Enemy->spawn(800, y, -5);
	listOfEnemyShips.push_back(p_Enemy);
}


void Game::engineFX() {
	Mix_PlayChannel(-1, gEngineFX, 0);
}

// Spawn Weapon at ships location
void Game::spawnLaser() {
	Laser* p_Laser = new Laser();
	p_Laser->spawn(player.getX(), player.getY());
	listOfLaserObjects.push_back(p_Laser);
	Mix_PlayChannel(-1, gLaserFX, 0);
}
void Game::spawnNinjaStar() {
	NinjaStar* p_NinjaStar = new NinjaStar();
	p_NinjaStar->spawn(player.getX(), player.getY());
	listOfNinjaStarObjects.push_back(p_NinjaStar);
	Mix_PlayChannel(-1, gNinjaFX, 0);
}

void Ship::render() {
	gShipTexture.render(player.getX(), player.getY());				// Show the ship
}

// Render the laser objects to the screen
void Laser::render() {
	gLaserTexture.render(getX(), getY());
}

/* Render the laser objects to the screen*/
void NinjaStar::render() {
	gNinjaStar.render(getX(), getY(), NULL, degrees, NULL, SDL_FLIP_NONE);
}

void EnemyShip::render() {
	gEnemyShipTexture.render(getX(), getY());
}
