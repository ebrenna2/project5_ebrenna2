#pragma once
//header file for sprite
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

//emma brennan
using namespace std;
class Sprite
{
	//friend functions for collided, endvalue, endgamevalue, sharkblock, boostblock1, 2, and 3
	friend int collided(int x, int y);
	friend bool endValue(int x, int y);
	friend bool endGameValue(int x, int y);
	friend bool sharkBlock(int x, int y);
	friend bool boostBlock1(int x, int y);
	friend bool boostBlock2(int x, int y);
	friend bool boostBlock3(int x, int y);
public:
	//setup for functions in sprite and booleans for public - all the sprite logic and block logic
	Sprite();
	~Sprite();
	void InitSprites(int width, int height, int xInit, int yInit);
	void UpdateSprites(int width, int height, int dir); //dir 1 = right, 0 = left, 2 = Standing Still
	void DrawSprites(int xoffset, int yoffset);
	bool CollideSprite();
	float getX() {return x;}
	float getY() {return y;}
	int getWidth() {return frameWidth;}
	int getHeight() {return frameHeight;}
	bool CollisionEndBlock();
	bool GameEndBlock();
	bool sharkCollision();
	bool block1();
	bool block2();
	bool block3();

	bool onBoostBlock1 = false;
	bool onBoostBlock2 = false;
	bool onBoostBlock3 = false;

private:
	//setup for private variables used in sprite
	float x;
	float y;
	int curFrameLR, maxFrameLR;
	int curFrameUD, maxFrameUD;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationRows;
	int animationDirection;
	float sharkCooldown;

	ALLEGRO_BITMAP *image;
};