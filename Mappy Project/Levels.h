#pragma once
//emma brennan
//levels header file
//including headers needed
#include "mappy_A5.h"
#include "SpriteSheet.h"
#include <allegro5/allegro_font.h>
//levels class declaration
class Levels {
public:
	//levels deconstructor aand setups
	Levels();
	~Levels();
	//function to initialize level elements
	void init(int initiallevel, int totalLevels, int screenWidth, int screenHeight);
	//function to load next level
	bool loadNextLevel(Sprite &player);
	//function to draw map
	void drawMap(int xOffset, int yOffset);
	//function to upddate map
	void updateMap(int xOffset, int yOoffset);
	//function to check if game is over
	bool isGameOver();
	//function to get the current level
	int getCurrentLevel();
	//function to reset timer
	void resetTimer();
	//function to get remaining time
	int getRemainingTime();
	//function to update timer
	void updateTimer();
	//function to draw timer to screen
	void drawTimer(ALLEGRO_DISPLAY* display);
	//deals with allegro events
	void events();

	//private variables for the level class
private:
	int currentLevel;
	int totalLevels;
	int screenWidth;
	int screenHeight;
	int timeLimit;
	int counter;
	bool gameOver;
	char levelName[20];
	ALLEGRO_FONT* font1;
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* event_queue;
};

