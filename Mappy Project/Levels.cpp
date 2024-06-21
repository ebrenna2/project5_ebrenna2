//emma brennan
//include all the header files needed
#include "Levels.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <cstdio>
//set up levels, currentlevel, total levels ,time limit, counter, gameover check, initialize lives to 5, coins collected to 0, shark encounters to 0
Levels::Levels() :currentLevel(1), totalLevels(3), timeLimit(60), counter(0), gameOver(false), playerLives(5), coinsCollected(0), sharkEncounters(0) {
    heartImage = al_load_bitmap("heart.png");
    //load the fonts
	font1 = al_load_ttf_font("Helvetica.ttf", 24, 0);
	font2 = al_load_ttf_font("Over.ttf", 60, 0);
    //make the timer (tick 1 second)
    timer = al_create_timer(1.0);
    //make the event queue
    event_queue = al_create_event_queue();
    //start the timer and do the register event sources
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    //start timer
    al_start_timer(timer);
}

//deconstructor
Levels::~Levels() {
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(heartImage);
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
    al_draw_text(font1, al_map_rgb(0, 0, 0), 0, 0, 0, timeText);
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

//draws the hearts to the screen
void Levels::drawHealthBar() {
//heart centers
    float cx = al_get_bitmap_width(heartImage) / 2;
    float cy = al_get_bitmap_height(heartImage) / 2;

    //for loop draws hearts for number of player lives
    for (int i = 0; i < playerLives; i++) {
        //hearts x position
        float x = 35 * i + cx;
        //heart y position
        float y = 450 + cy;
        //scale and rotation
        float scale = 1.0f;
        float rotation = 0.0f;

        //apply scaling and rotation effects based on the number of player lives
        if (playerLives == 3) {
            //smallest scaling and rotation
            scale = 1.0f + 0.05f * sin(al_get_time() * 5);
            rotation = 0.05f * sin(al_get_time() * 5);
        }

        //medium scale and rotation
        else if (playerLives == 2) {
            scale = 1.0f + 0.1f * sin(al_get_time() * 5);
            rotation = 0.1f * sin(al_get_time() * 5);
        }

        //most scaling and rotating (big heartbeat)
        else if (playerLives == 1) {
            scale = 1.0f + 0.2f * sin(al_get_time() * 5);
            rotation = 0.2f * sin(al_get_time() * 5);
        }

        //draw the scaled and rotated heart
        al_draw_scaled_rotated_bitmap(heartImage, cx, cy, x, y, scale, scale, rotation, 0);
    }
}

//decrement user lives
void Levels::decrementLives() {
    //increase shark encounter for end of game stats
    sharkEncounters++;
    //decrement user lives
    playerLives--;
    //if lives go to 0, end game
    if (playerLives <= 0) {
        gameOver = true;
    }
}

//increment user lives
void Levels::incrementLives() {
    //if user lives are less than max lives, increment life by 1
    if (playerLives < MAX_LIVES) {
        playerLives++;
    }
}

//collect coin, for end of game stats to show how many boosts they got
void Levels::collectCoin() {
    coinsCollected++;
}

//get player lives, returns the player lives
int Levels::getPlayerLives() const {
    return playerLives;
}

//displays the stats for the end of the game
void Levels::displayStats(ALLEGRO_FONT* font2, int width, int height) {
    //for storing stats message
    char stats[100];
    //formats stats message with heartsleft, foodcollected, sharksencountered
    sprintf(stats, "Hearts Left: %d\nFish food collected: %d\nSharks Encountered (bit you): %d", playerLives, coinsCollected, sharkEncounters);
    //clear to black
    al_clear_to_color(al_map_rgb(0, 0, 0));
    //draws the congrats text to the screen
    al_draw_text(font2, al_map_rgb(255, 255, 255), width / 2, height / 2 - 50, ALLEGRO_ALIGN_CENTER, "Congratulations! You Won! Fishy made it safely.");
    //draws the stats in multilines with stats 
    al_draw_multiline_text(font2, al_map_rgb(255, 255, 255), width / 2, height / 2 - 10, width - 40, 20, ALLEGRO_ALIGN_CENTER, stats);
    //flips the display
    al_flip_display();
}

//displays game over screen when user dies
void Levels::displayGameOver(ALLEGRO_FONT* font, int width, int height) {
    //clears to black
    al_clear_to_color(al_map_rgb(0, 0, 0));
    //draws text that they died
    al_draw_text(font2, al_map_rgb(255, 255, 255), width / 2, height / 2, ALLEGRO_ALIGN_CENTER, "You died! Game over. Try again!");
    //flip display
    al_flip_display();
    //rest for 10 seconds
    al_rest(10);
}



