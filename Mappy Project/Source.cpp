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
int collided(int x, int y); 
bool newLevelValue(int x, int y); 

int main(void) {
    const int WIDTH = 900;
    const int HEIGHT = 480;
    bool keys[] = { false, false, false, false, false };
    enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };

    bool done = false;
    bool render = false;

    Sprite player;
    const int JUMPIT = 1600;

    // Initialize Allegro
    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_FONT* font = al_load_ttf_font("AppleGaramond.ttf", 36, 0);

    // Initialize levels
    Levels levels;
    levels.init(1, 3, WIDTH, HEIGHT);

    // Initialize timers
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* fps_timer = al_create_timer(1.0 / 60);
    ALLEGRO_TIMER* game_timer = al_create_timer(1.0);

    // Register event sources
    al_register_event_source(event_queue, al_get_timer_event_source(fps_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(game_timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // Start timers
    al_start_timer(fps_timer);
    al_start_timer(game_timer);

    player.InitSprites(WIDTH, HEIGHT, 0, 195);

    int xOff = 0, yOff = 0;

    // Game loop
    while (!done) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (ev.timer.source == fps_timer) {
                // Handle sprite movement based on keyboard input
                if (!levels.isGameOver()) {
                    if (keys[DOWN]) player.UpdateSprites(WIDTH, HEIGHT, 3);
                    if (keys[LEFT]) player.UpdateSprites(WIDTH, HEIGHT, 0);
                    if (keys[RIGHT]) player.UpdateSprites(WIDTH, HEIGHT, 1);
                    if (keys[UP]) player.UpdateSprites(WIDTH, HEIGHT, 2);

                    render = true;
                }
            }
            if (ev.timer.source == game_timer) {
                // Update the game time every second
                levels.updateTimer();
                if (levels.isGameOver()) {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Game Over");
                    al_flip_display();
                    al_rest(2.0);
                    done = true;
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_UP:
                keys[UP] = true;
                break;
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = true;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = true;
                break;
            case ALLEGRO_KEY_SPACE:
                keys[SPACE] = true;
                break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_UP:
                keys[UP] = false;
                break;
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = false;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = false;
                break;
            case ALLEGRO_KEY_SPACE:
                keys[SPACE] = false;
                break;
            }
        }
        if (render && al_is_event_queue_empty(event_queue)) {
            render = false;
            MapUpdateAnims();
            if (!levels.isGameOver()) {
                xOff = player.getX() + player.getWidth() - WIDTH / 2;
                yOff = player.getY() + player.getHeight() - HEIGHT / 2;


                if (xOff < 0) xOff = 0;
                if (xOff > (mapwidth * mapblockwidth - WIDTH)) xOff = mapwidth * mapblockwidth - WIDTH;
                if (yOff < 0) yOff = 0;
                if (yOff > (mapheight * mapblockheight - HEIGHT)) yOff = mapheight * mapblockheight - HEIGHT;
                al_clear_to_color(al_map_rgb(0, 0, 0));
                MapDrawBG(xOff, yOff, 0, 0, WIDTH, HEIGHT);

                MapDrawFG(xOff, yOff, 0, 0, WIDTH, HEIGHT, 0);


                player.DrawSprites(xOff, yOff);

                levels.drawTimer(display);
            }
            else {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Game Over");
            }
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

int collided(int x, int y)
{
    BLKSTR* blockdata;
    blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
    return blockdata->tl;
}

bool newLevelValue(int x, int y)
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
