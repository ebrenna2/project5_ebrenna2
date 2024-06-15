#include "SpriteSheet.h"
//emma brennan
Sprite::Sprite()
{
	image=NULL;
}
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}
void Sprite::InitSprites(int width, int height, int xInit, int yInit)
{
	x = xInit;
	y = yInit;


	maxFrame = 2;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 50;
	frameHeight = 32;
	animationColumns = 6;
	animationDirection = 1;

	image = al_load_bitmap("erm.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255,0,255));
}

void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;
	bool isMoving = false;

	if (dir == 1) { //right key
		animationDirection = 1;
		x += 2;
		isMoving = true;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame)
				curFrame = 1;
		}
	}
	else if (dir == 0) { //left key
		animationDirection = 0;
		isMoving = true;
		x -= 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame)
				curFrame = 1;
		}
	}
	else if (dir == 2) { //up key
		animationDirection = 0;
		y -= 2;
		isMoving = true;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame)
				curFrame = 4;
		}
	}
	else if (dir == 3) { //down key
		animationDirection = 0;
		y += 2;
		isMoving = true;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame)
				curFrame = 4;
		}
	}
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
		if (collided(x, y)) { //collision detection to the up
			x = oldx;
			y = oldy;
		}
	}

	else if (animationDirection == 3)
	{
		if (collided(x, y + frameHeight)) { //collision detection to the down
			x = oldx;
			y = oldy;
		}
	}

	if (y + frameHeight > height) {
		y = height - frameHeight;
	}

	if (CollideSprite()) {
        x = oldx;
        y = oldy;
    }

	if (y + frameHeight > height) {
		y = height - frameHeight;
	}
}

bool Sprite::CollisionEndBlock()
{
	if (endValue(x + frameWidth/2, y + frameHeight + 5))
		return true;
	else
		return false;
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;

	if (animationDirection==1){
		al_draw_bitmap_region(image, fx, fy, frameWidth,frameHeight, x-xoffset, y-yoffset, 0);
	}else if (animationDirection == 0 ){
		al_draw_bitmap_region(image, fx, fy, frameWidth,frameHeight, x-xoffset, y-yoffset, ALLEGRO_FLIP_HORIZONTAL);
	}else if (animationDirection == 2 ){
		al_draw_bitmap_region(image,0,0,frameWidth,frameHeight,  x-xoffset, y-yoffset, ALLEGRO_FLIP_VERTICAL);

	}

	else if (animationDirection == 3) {
		al_draw_bitmap_region(image, 0, 0, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);

	}
}

bool Sprite::CollideSprite() {
	if (collided(x, y + frameHeight) || collided(x + frameWidth, y + frameHeight) || collided(x + frameWidth / 2, y + frameHeight / 2))
	{
		return true;
	}

	return false;
}

