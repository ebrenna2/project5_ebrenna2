#ifndef LEVELS_H
#define LEVELS_H

#include "mappy_A5.h"
#include <allegro5/allegro_font.h>
class Levels {
public:
	Levels();
	~Levels();
	void init(int initiallevel, int totalLevels, int screenWidth, int screenHeight);
	bool loadNextLevel();
	void drawMap(int xOffset, int yOffset);
	void updateMap(int xOffset, int yOoffset);
	bool isGameOver();
	int getCurrentLevel();
	void resetTimer();
	int getRemainingTime();
	void updateTimer();
	void drawTimer(ALLEGRO_DISPLAY* display);
	void events();

private:
	int currentLevel;
	int totalLevels;
	int screenWidth;
	int screenHeight;
	int timeLimit;
	int counter;
	bool gameOver;
	char levelName[20];
	ALLEGRO_FONT* font;
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* event_queue;
};

#endif