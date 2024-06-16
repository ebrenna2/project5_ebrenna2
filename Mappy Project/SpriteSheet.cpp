#include "SpriteSheet.h"
//emma brennan
//setup - image is null
Sprite::Sprite()
{
	image=NULL;
}
//deconstructor
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}

//all the sprite class components initialization, its width, height, initial x and y positions, current frames (L/R and U/D), maximum frames
//for all directions, bitmap thats being used, and removing the bitmaps background
void Sprite::InitSprites(int width, int height, int xInit, int yInit)
{
	x = xInit;
	y = yInit;

	curFrameLR = 0;
	maxFrameLR = 2;
	curFrameUD = 3;
	maxFrameUD = 4;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 46;
	frameHeight = 32;
	animationColumns = 5;
	animationDirection = 1;

	image = al_load_bitmap("newfishy.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255,0,255));
}

//updating the sprite for when a certain key is pressed... its frames, current frames, and direction it should point in
void Sprite::UpdateSprites(int width, int height, int dir)
{
	//old x, old y, and boolean to check if it is moving
	int oldx = x;
	int oldy = y;
	bool isMoving = false;

	if (dir == 1) { //right key
		animationDirection = 1;
		x += 2;
//set ismoving to true
		isMoving = true;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrameLR > maxFrameLR)
				curFrameLR = 1;
		}
	}
	else if (dir == 0) { //left key
		//set ismoving to true
		animationDirection = 0;
		//set ismoving to true
		isMoving = true;
		x -= 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrameLR > maxFrameLR)
				curFrameLR = 1;
		}
	}
	else if (dir == 2) { //up key
		animationDirection = 2;
		y -= 2;
		//set ismoving to true
		isMoving = true;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrameUD > maxFrameUD)
				curFrameUD = 3;
		}
	}
	else if (dir == 3) { //down key
		animationDirection = 3;
		y += 2;
		//set ismoving to true
		isMoving = true;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrameUD > maxFrameUD)
				curFrameUD = 3;
		}
	}

	//if its not moving just gets whatever dir it was in
	if (!isMoving) {
		animationDirection = dir;
	}

	//check for collided with foreground tiles
	if (animationDirection==0)
	{ 
		if (collided(x, y + frameHeight)) { //collision detection to the left
			x = oldx; 
			y= oldy;
		}

	}
	else if (animationDirection ==1)
	{ 
		if (collided(x + frameWidth, y + frameHeight)) { //collision detection to the right
			x = oldx; 
			y= oldy;
		}
	}

	else if (animationDirection == 2)
	{
		if (collided(x, y)) { //collision detection up
			x = oldx;
			y = oldy;
		}
	}

	else if (animationDirection == 3)
	{
		if (collided(x, y + frameHeight)) { //collision detection down
			x = oldx;
			y = oldy;
		}
	}

	//checks bottom boundary
	if (y + frameHeight > height) {
		y = height - frameHeight;
	}

	//checks forr collidesprite - checks for collision and goes back to oldx and oldy
	if (CollideSprite()) {
        x = oldx;
        y = oldy;
    }
}

//gets the value for the collision end block (for the next level), checks if sprite is there
bool Sprite::CollisionEndBlock()
{
	if (endValue(x + frameWidth/2, y + frameHeight + 5))
		return true;
	else
		return false;
}

//gets the value for the game end block (for the game ending), checks if sprite is there
bool Sprite::GameEndBlock()
{
	if (endGameValue(x + frameWidth / 2, y + frameHeight + 5))
		return true;
	else
		return false;
}

//draws the sprite to the screen based on the direction
void Sprite::DrawSprites(int xoffset, int yoffset)
{
	//x and y
	int fx, fy;

	// Determine the frame to use based on direction
	if (animationDirection == 0 || animationDirection == 1) { // Left or Right
		fx = (curFrameLR % animationColumns) * frameWidth;
		fy = (curFrameLR / animationColumns) * frameHeight;
	}
	else if (animationDirection == 2 || animationDirection == 3) { // Up or Down
		fx = (curFrameUD % animationColumns) * frameWidth;
		fy = (curFrameUD / animationColumns) * frameHeight;
	}

	// Draw the sprite based on the current animation direction
	if (animationDirection == 1) { // Moving right
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
	else if (animationDirection == 0) { // Moving left
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, ALLEGRO_FLIP_HORIZONTAL);
	}
	else if (animationDirection == 2) { // Moving up
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
	else if (animationDirection == 3) { // Moving down
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, ALLEGRO_FLIP_VERTICAL);
	}
}

//checks if sprite collided
bool Sprite::CollideSprite() {
	if (collided(x, y + frameHeight) || collided(x + frameWidth, y + frameHeight) || collided(x + frameWidth / 2, y + frameHeight / 2))
	{
		return true;
	}

	return false;
}

