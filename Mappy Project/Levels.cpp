//emma brennan
//include all the header files needed
#include "Levels.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <cstdio>

//set up levels, currentlevel, total levels ,tiem limit, counter, gameover check
Levels::Levels() :currentLevel(1), totalLevels(3), timeLimit(60), counter(0), gameOver(false) {
    //load the font
	font1 = al_load_ttf_font("AppleGaramond.ttf", 36, 0);
    //make the timer (tick 1 second)
    timer = al_create_timer(1.0);
    //make the event queue
    event_queue = al_create_event_queue();
    //start the timer and do the register event sources
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
}

//deconstructor
Levels::~Levels() {
    al_destroy_font(font1);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}

//initialize everything (using this->, 
void Levels::init(int initialLevel, int totalLevels, int screenWidth, int screenHeight) {
    this->currentLevel = initialLevel;
    this->totalLevels = totalLevels;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    //formats level name so it gets level name and current level and puts it into the %d to load the respective bitmap
    sprintf(levelName, "rivermaze%d.fmp", currentLevel);
    //load the level and initialize the current level, if failed do exit(-5)
    if (MapLoad(levelName, currentLevel)) exit(-5);
}

//deals with loading the next level, takes the player in as a parameter to know where to initialize it
bool Levels::loadNextLevel(Sprite &player) {
    //increment the currentlevel
    currentLevel++;
    //if the currentlevel is greater than total levels, if so its gameover and returns false bc theres nothing else to load
    if (currentLevel > totalLevels) {
        gameOver = true;
        return false;
    }

    //formats level name so it gets level name and current level and puts it into the %d to load the respective bitmap
    sprintf(levelName, "rivermaze%d.fmp", currentLevel);
    //tries to load the map
    if (MapLoad(levelName, 1)) exit(-5);
    //resets timer when a new map is loaded
    resetTimer();
    //initializes the sprite back onto the right part of the screen
    player.InitSprites(screenWidth, screenHeight, 0, 195);
    //returns true for success
    return true;
}

//draws the map to the screen (mostly already do this in the source.cpp file)
void Levels::drawMap(int xOffset, int yOffset) {
    MapDrawBG(xOffset, yOffset, 0, 0, screenWidth - 1, screenHeight - 1);
    MapDrawFG(xOffset, yOffset, 0, 0, screenWidth - 1, screenHeight - 1, 0);
}

//updates the map
void Levels::updateMap(int xOffset, int yOffset) {
    drawMap(xOffset, yOffset);
}

//checks if the game is over
bool Levels::isGameOver() {
    return gameOver;
}

//gets the current level that the player is on
int Levels::getCurrentLevel() {
    return currentLevel;
}

//resets the timer after the next level is reached
void Levels::resetTimer() {
    counter = 0;
}

//gets the remaining time on the timer
int Levels::getRemainingTime() {
    return counter;
}

//updates the timer
void Levels::updateTimer() {
    //increments the counter, then if it is greater than or equal to time limit, game over
    counter++;
    if (counter >= timeLimit) {
        gameOver = true;
    }
}

//draws the timer to the screen by getting the remaining time, char for the time: text, draws it to the screen.
void Levels::drawTimer(ALLEGRO_DISPLAY* display) {
    int remainingTime = getRemainingTime();
    char timeText[10];
    sprintf(timeText, "Time: %d", remainingTime);
    al_draw_text(font1, al_map_rgb(0, 0, 0), 0, 30, 0, timeText);
}

//deals with the level events, while theres a next event, deals with the allegro timer and updates it
void Levels::events() {
    ALLEGRO_EVENT event;
    while (al_get_next_event(event_queue, &event))
    {
        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            updateTimer();
        }
    }
}
