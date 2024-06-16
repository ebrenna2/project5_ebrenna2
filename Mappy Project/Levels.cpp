#include "Levels.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <cstdio>

Levels::Levels() :currentLevel(1), totalLevels(3), timeLimit(60), counter(0), gameOver(false) {
	font = al_load_ttf_font("AppleGaramond", 36, 0);
}

void Levels::init(int initialLevel, int totalLevels, int screenWidth, int screenHeight) {
    this->currentLevel = initialLevel;
    this->totalLevels = totalLevels;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    sprintf(levelName, "rivermaze%i.FMP", currentLevel);
    if (MapLoad(levelName, currentLevel)) exit(-5);
}

bool Levels::loadNextLevel() {
    currentLevel++;
    if (currentLevel > totalLevels) {
        gameOver = true;
        return false;
    }
    sprintf(levelName, "rivermaze%i.FMP", currentLevel);
    if (MapLoad(levelName, 1)) exit(-5);
    resetTimer();
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
    return timeLimit - counter;
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
    sprintf(timeText, "Time: %d", remainingTime);
    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, timeText);
}
