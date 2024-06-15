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
void Sprite::InitSprites(int width, int height)
{
	x = 80;
	y = -10;


	maxFrame = 8;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 50;
	frameHeight = 64;
	animationColumns = 8;
	animationDirection = 1;

	image = al_load_bitmap("fishies.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255,0,255));
}

void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;
	bool isMoving = false;

	if(dir == 1){ //right key
		animationDirection = 1; 
		x+=2; 
		if (++frameCount > frameDelay)
		{
			frameCount=0;
			if (++curFrame > maxFrame)
				curFrame=1;
		}
	} else if (dir == 0){ //left key
		animationDirection = 0; 
		x-=2; 
		if (++frameCount > frameDelay)
		{
			frameCount=0;
			if (++curFrame > maxFrame)
				curFrame=1;
		}
	}

	else if (dir == 2) { // Climbing right
		animationDirection = 1;
		if (climbCollide()) {
			if (++frameCount > frameDelay)
			{
				frameCount = 0;
				if (++curFrame > maxFrame)
					curFrame = 1;
				y -= 32;
				x += 32;
			}
		}
	}
	else if (dir == 3) { // Climbing left
		animationDirection = 0;
		if (climbCollide()) {
			if (++frameCount > frameDelay)
			{
				frameCount = 0;
				if (++curFrame > maxFrame)
					curFrame = 2;
				y -= 32;  
				x -= 32;  
			}
		}
	}

	else if (dir == 6) { // Climbing down and right
		animationDirection = 1;
		if (climbCollide()) {
			if (++frameCount > frameDelay)
			{
				frameCount = 0;
				if (++curFrame > maxFrame)
					curFrame = 1;
				y += 32; // Move down
				x += 32; // Move right
			}
		}
	}
	else if (dir == 7) { 
		animationDirection = 0; 
		if (climbCollide()) {
			if (++frameCount > frameDelay)
			{
				frameCount = 0;
				if (++curFrame > maxFrame)
					curFrame = 2;
				y += 32; // Move down
				x -= 32; // Move left
			}
		}
	}

	else if (dir == 4) {
		if (throughCollide()) {
			y += 36;
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

	if (y + frameHeight > height) {
		y = height - frameHeight;
	}

	if (y < 32) {
		y = 33;
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
		al_draw_bitmap_region(image,0,0,frameWidth,frameHeight,  x-xoffset, y-yoffset, 0);

	}
}


int Sprite::jumping(int jump, const int JUMPIT)
{
	//handle jumping
	if (jump == JUMPIT) {
		if (!collided(x + frameWidth / 2, y + frameHeight + 5))
			jump = 0;
	}
	else
	{
		y -= jump / 3;
		jump--;
		curFrame = 9;
	}

	if (jump < 0)
	{
		if (collided(x + frameWidth / 2, y + frameHeight))
		{
			jump = JUMPIT;
			while (collided(x + frameWidth / 2, y + frameHeight))
			{
				y -= 3;
			}
		}
	}
	return jump;
}

bool Sprite::throughCollide()
{
	if (throughValue(x + frameWidth / 2, y + frameHeight + 5))
		return true;
	else
		return false;
}

bool Sprite::climbCollide()
{
	if (climbValue(x + frameWidth / 2, y + frameHeight + 5))
		return true;
	else
		return false;
}