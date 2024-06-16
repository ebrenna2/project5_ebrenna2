#include "Levels.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <cstdio>

Levels::Levels() :currentLevel(1), totalLevels(3), timeLimit(60), counter(0), gameOver(false) {
	font1 = al_load_ttf_font("AppleGaramond.ttf", 36, 0);
    timer = al_create_timer(1.0);
    event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
}

Levels::~Levels() {
    al_destroy_font(font1);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}

void Levels::init(int initialLevel, int totalLevels, int screenWidth, int screenHeight) {
    this->currentLevel = initialLevel;
    this->totalLevels = totalLevels;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    sprintf(levelName, "rivermaze%d.fmp", currentLevel);
    if (MapLoad(levelName, currentLevel)) exit(-5);
}

bool Levels::loadNextLevel(Sprite &player) {
    currentLevel++;
    if (currentLevel > totalLevels) {
        gameOver = true;
        return false;
    }
    sprintf(levelName, "rivermaze%d.fmp", currentLevel);
    if (MapLoad(levelName, 1)) exit(-5);
    resetTimer();
    player.InitSprites(screenWidth, screenHeight, 0, 195);
    return true;
}

void Levels::drawMap(int xOffset, int yOffset) {
    MapDrawBG(xOffset, yOffset, 0, 0, screenWidth - 1, screenHeight - 1);
    MapDrawFG(xOffset, yOffset, 0, 0, screenWidth - 1, screenHeight - 1, 0);
}

void Levels::updateMap(int xOffset, int yOffset) {
    MapUpdateAnims();
    drawMap(xOffset, yOffset);
}

bool Levels::isGameOver() {
    return gameOver;
}

int Levels::getCurrentLevel() {
    return currentLevel;
}

void Levels::resetTimer() {
    counter = 0;
}

int Levels::getRemainingTime() {
    return counter;
}

void Levels::updateTimer() {
    counter++;
    if (counter >= timeLimit) {
        gameOver = true;
    }
}

void Levels::drawTimer(ALLEGRO_DISPLAY* display) {
    int remainingTime = getRemainingTime();
    char timeText[10];
    char totalTimeText[20];
    sprintf(timeText, "Time: %d", remainingTime);
    al_draw_text(font1, al_map_rgb(0, 0, 0), 0, 30, 0, timeText);
}

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
