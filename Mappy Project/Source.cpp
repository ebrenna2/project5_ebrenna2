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
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
using namespace std;
//emma brennan
//functions defs for colliding,  endvalue, endgamevalue, sharkblock, boostblocks 1, 2, and 3 for the 3 diff levels
int collided(int x, int y); 
bool endValue(int x, int y);
bool endGameValue(int x, int y);
bool sharkBlock(int x, int y);
bool boostBlock1(int x, int y);
bool boostBlock2(int x, int y);
bool boostBlock3(int x, int y);

int main(void) {
    //initialize width and height, the keys
    const int WIDTH = 900;
    const int HEIGHT = 480;
    bool keys[] = { false, false, false, false, false };
    enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };

    //booleans for done and render
    bool done = false;
    bool render = false;

    Sprite player;


    // Initialize Allegro and its addons
    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();

    //make the audio - wii for background music
    ALLEGRO_SAMPLE* sample = NULL;
    //reserve samples
    al_reserve_samples(8);
    sample = al_load_sample("wii.OGG");
   
    //load all the other samples (theres a lot)
    ALLEGRO_SAMPLE* chomp = NULL;
    chomp = al_load_sample("chomp.OGG");
    ALLEGRO_SAMPLE* yay = NULL;
    yay = al_load_sample("yay.OGG"); 
    ALLEGRO_SAMPLE* womp = NULL;
    womp = al_load_sample("womp.OGG");
    ALLEGRO_SAMPLE* oof = NULL;
    oof = al_load_sample("oof.OGG"); 
    ALLEGRO_SAMPLE* level = NULL;
    level = al_load_sample("level.OGG"); 
    ALLEGRO_SAMPLE* boost = NULL;
    boost = al_load_sample("boost.OGG");
    //create the display
    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    //create the font
    ALLEGRO_FONT* font = al_load_ttf_font("Over.ttf", 60, 0);
    //play the background music on a loop
    al_play_sample(sample, 0.5, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
   
    // Initialize levels - num 1 first, 3 total
    Levels levels;
    levels.init(1, 3, WIDTH, HEIGHT);

    //load the welcome image as a bitmap
    ALLEGRO_BITMAP* welcome_img = al_load_bitmap("welcome.png");

    // Initialize timers and event queue - oen timer for fps and one for the game
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* fps_timer = al_create_timer(1.0 / 60);
   
    // Register event sources, fps timer and game timer, and keyboard
    al_register_event_source(event_queue, al_get_timer_event_source(fps_timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // Start timers
    al_start_timer(fps_timer);

    //initialize the sprite and its x and y position
    player.InitSprites(WIDTH, HEIGHT, 0, 195);

    //variables for scrolling
    int xOff = 0, yOff = 0;

    //clear screen to black
    al_clear_to_color(al_map_rgb(0, 0, 0));
    //draw the welcome image to the screen
    al_draw_bitmap(welcome_img, 0, 0, 0);
    //flip display
    al_flip_display();
    //create event queue for the welcome queue and register event
    ALLEGRO_EVENT_QUEUE* welcome_queue = al_create_event_queue();
    al_register_event_source(welcome_queue, al_get_keyboard_event_source());

    //show the welcome screen then wait for when the user presses enter to start the game
    while (true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(welcome_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            break;
        }
    }

    //destroy the bitmap and event queue for the welcome image
    al_destroy_bitmap(welcome_img);
    al_destroy_event_queue(welcome_queue);

    //create the game timer (sorry everything is all over the place)
    ALLEGRO_TIMER* game_timer = al_create_timer(1.0);
    //register the game timer
    al_register_event_source(event_queue, al_get_timer_event_source(game_timer));
    //start the game timer
    al_start_timer(game_timer);

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

                    //if the player collides with the shark block
                    if (player.sharkCollision())
                    {
                        //play the chomp  and oof noise
                        al_play_sample(chomp, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        al_play_sample(oof, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        //decrement the player's lives
                        levels.decrementLives();
                        //if their lives are less than or equal to 0
                        if (levels.getPlayerLives() <= 0) {
                            //play womp womp noise
                            al_play_sample(womp, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            //display the game over screen
                            levels.displayGameOver(font, WIDTH, HEIGHT);
                            //done is equal to true
                            done = true;
                        }
                    }
                    
                    //if the player collides with any of the booster blocks
                    if (player.block1() || player.block2() || player.block3()) {
                        //check if player lives is less than max lives, which is 5
                        if (levels.getPlayerLives() < MAX_LIVES) {
                            //play the boost noise
                            al_play_sample(boost, 0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            //increment life (+ 1)
                            levels.incrementLives();
                            //call collect coin for end of game stats
                            levels.collectCoin();
                        }
                    }

                    //if collided with the end block on a level
                    if (player.CollisionEndBlock())
                    {
                        //play the call of duty level up noise lol
                        al_play_sample(level, 0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        //load next level
                        if (!levels.loadNextLevel(player)) {
                            done = true;
                        }
                    }

                    //if the player reached the end game block, put a gameover screen with stats, flip display, rest, then game ends
                    if (player.GameEndBlock())
                    {
                        //play yayyyyyyy sound
                        al_play_sample(yay, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        //display stats
                        levels.displayStats(font, WIDTH, HEIGHT);
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
                    //play womp womp
                    al_play_sample(womp, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    //clear to black
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    //draw game over on screen, flip display, rest, game ends
                    al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Time's up! Game over. Try again!");
                    al_flip_display();
                    al_rest(10);
                    done = true;
                }
            }
        }

        //close display 
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
            MapUpdateAnims();
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
                levels.drawHealthBar();
                //draw the timer to the display
                levels.drawTimer(display);
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
    al_destroy_sample(yay);
    al_destroy_sample(womp);
    al_destroy_sample(sample);
    al_destroy_sample(oof);
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

//check for the shark block
bool sharkBlock(int x, int y)
{
    BLKSTR* data;
    data = MapGetBlock(x /mapblockwidth, y / mapblockheight);
    if (data->user1 == 12)
    {
        return true;
    }
    else
        return false;
}

//check for the first boost block, then disable (user2->1, when collided with)
bool boostBlock1(int x, int y)
{
    BLKSTR* data;
    data = MapGetBlock(x / mapblockwidth, y / mapblockheight);
    if (data->user1 == 14 && data->user2 == 0)
    {
        data->user2 = 1;
        return true;
    }
    return false;
}

//check for the second boost block, then disable (user2->1, when collided with)
bool boostBlock2(int x, int y)
{
    BLKSTR* data;
    data = MapGetBlock(x / mapblockwidth, y / mapblockheight);
    if (data->user1 == 15 && data->user2 == 0)
    {
        data->user2 = 1;
        return true;
    }
    return false;
}

//check for the third boost block, then disable (user2->1, when collided with)
bool boostBlock3(int x, int y)
{
    BLKSTR* data;
    data = MapGetBlock(x / mapblockwidth, y / mapblockheight);
    if (data->user1 == 16 && data->user2 == 0)
    {
        data->user2 = 1;
        return true;
    }
    return false;
}