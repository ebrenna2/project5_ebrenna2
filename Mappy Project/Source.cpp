//include all necessary header files
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "SpriteSheet.h"
#include "mappy_A5.h"
#include <iostream>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "Levels.h"
using namespace std;
//emma brennan
//functions defs for colliding,  endvalue, and endgamevalue
int collided(int x, int y); 
bool endValue(int x, int y);
bool endGameValue(int x, int y);

int main(void) {
    //initialize width and height, the keys
    const int WIDTH = 900;
    const int HEIGHT = 480;
    bool keys[] = { false, false, false, false, false };
    enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };

    bool done = false;
    bool render = false;

    Sprite player;
    const int JUMPIT = 1600;

    // Initialize Allegro and its addons
    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    //create the display and allegro font
    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_FONT* font = al_load_ttf_font("AppleGaramond.ttf", 24, 0);

    // Initialize levels - num 1 first, 3 total
    Levels levels;
    levels.init(1, 3, WIDTH, HEIGHT);

    // Initialize timers and event queue - oen timer for fps and one for the game
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* fps_timer = al_create_timer(1.0 / 60);
    ALLEGRO_TIMER* game_timer = al_create_timer(1.0);

    // Register event sources, fps timer and game timer, and keyboard
    al_register_event_source(event_queue, al_get_timer_event_source(fps_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(game_timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // Start timers
    al_start_timer(fps_timer);
    al_start_timer(game_timer);

    //initialize the sprite and its x and y position
    player.InitSprites(WIDTH, HEIGHT, 0, 195);

    //variables for scrolling
    int xOff = 0, yOff = 0;
    // Game loop
    while (!done) {
        //wait for allegro event
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        //if its the timer
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            //timer source is from fps timer
            if (ev.timer.source == fps_timer) {
                // Handle sprite movement based on keyboard input
                //while its not game over, deal with keyboard (arrow) presses
                if (!levels.isGameOver()) {
                    //if down arrow key, update sprite to move down
                    if (keys[DOWN]) player.UpdateSprites(WIDTH, HEIGHT, 3);
                    //if left arrow, update sprite to move left but do a check to make sure it doesnt go all the way past the left side of screen
                    if (keys[LEFT]) {
                        if (player.getX() > 0) {
                            player.UpdateSprites(WIDTH, HEIGHT, 0);
                        }
                    }
                    //if right arrow key, update the sprite to move right
                    if (keys[RIGHT]) player.UpdateSprites(WIDTH, HEIGHT, 1);
                    //if up arrow key, update sprite to move up
                    if (keys[UP]) player.UpdateSprites(WIDTH, HEIGHT, 2);

                    //render
                    render = true;

                    //check if the player reached the collision block for the next level, if so load the next level - done with that level
                    if (player.CollisionEndBlock())
                    {
                        if (!levels.loadNextLevel(player)) {
                            done = true;
                        }
                    }

                    //if the player reached the end game block, put a gameover screen, say player, flip display, rest, then game ends
                    if (player.GameEndBlock())
                    {
                        al_clear_to_color(al_map_rgb(0, 0, 0));
                        al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Yay! You Win! Fishy made it!");
                        al_flip_display();
                        al_rest(10);
                        done = true;
                    }

                }
            }
            //deals with the game timer
            if (ev.timer.source == game_timer) {
                // Update the game time every second
                levels.updateTimer();
                //if the player uses all their time for a given level, says it is game over
                if (levels.isGameOver()) {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Time's up! Game Over");
                    al_flip_display();
                    al_rest(10);
                    done = true;
                }
            }
        }

        //close display if user closes it
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        }
        //deals with pressing keys down and what to do when that occurs
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                //escape
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
                //up
            case ALLEGRO_KEY_UP:
                keys[UP] = true;
                break;
                //down
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = true;
                break;
                //left
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = true;
                break;
                //right
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = true;
                break;
                //space (not used)
            case ALLEGRO_KEY_SPACE:
                keys[SPACE] = true;
                break;
            }
        }
        //what happens when the key is released
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
                //esc
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
                //up
            case ALLEGRO_KEY_UP:
                keys[UP] = false;
                break;
                //down
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = false;
                break;
                //left
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = false;
                break;
                //right
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = false;
                break;
                //space (not used)
            case ALLEGRO_KEY_SPACE:
                keys[SPACE] = false;
                break;
            }
        }
        //what happens when the event queue is empty
        if (render && al_is_event_queue_empty(event_queue)) {
            //render set to false
            render = false;
            //deals with when game isnt over
            if (!levels.isGameOver()) {
                //scrolling and following player
                xOff = player.getX() + player.getWidth() - WIDTH / 2;
                yOff = player.getY() + player.getHeight() - HEIGHT / 2;

                //prevents scrolling off the screen
                if (xOff < 0) xOff = 0;
                if (xOff > (mapwidth * mapblockwidth - WIDTH)) xOff = mapwidth * mapblockwidth - WIDTH;
                if (yOff < 0) yOff = 0;
                if (yOff > (mapheight * mapblockheight - HEIGHT)) yOff = mapheight * mapblockheight - HEIGHT;
                //clears to black
                al_clear_to_color(al_map_rgb(0, 0, 0));
                //drawing background and foreground
                MapDrawBG(xOff, yOff, 0, 0, WIDTH, HEIGHT);

                MapDrawFG(xOff, yOff, 0, 0, WIDTH, HEIGHT, 0);

                //draw sprite
                player.DrawSprites(xOff, yOff);

                //draw the timer to the display
                levels.drawTimer(display);
                //draw that user has 60 seconds per level
                al_draw_text(font, al_map_rgb(0, 0, 0), 0, 0, 0, "60 seconds per level");
            }
            else {
                //otherwise (if user uses all the time), game is over and ends games
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), 0, 2, ALLEGRO_ALIGN_LEFT, "Game Over");
            }
            //flip the display
            al_flip_display();
        }

    }

    // Cleanup
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(fps_timer);
    al_destroy_timer(game_timer);
    return 0;
}

//check for collision blocks
int collided(int x, int y)
{
    BLKSTR* blockdata;
    blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
    return blockdata->tl;
}

//check for when the user gets to the end of the level and touches the top of the endlevel block, user1 -> 10 for that
bool endValue(int x, int y)
{
    BLKSTR* data;
    data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

    if (data->user1 == 10)
    {
        return true;
    }
    else
        return false;
}

//check for when the user gets to the last level end block, which is user1 -> 11
bool endGameValue(int x, int y)
{
    BLKSTR* data;
    data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

    if (data->user1 == 11)
    {
        return true;
    }
    else
        return false;
}