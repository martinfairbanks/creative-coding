/*	3D starfield
	by Martin Fairbanks
*/
	#include "framework\creativeframework.cpp"
	#define MAXSTARS 600

	struct Star
	{
		real32 xpos, ypos;
		int zpos, speed;
		uint8 color;
	} stars[MAXSTARS];

	static int centerX;
	static int centerY;
	static real32 prevPosX;
	static real32 prevPosY;

	void initStar(Star* star, int i)
	{
		//randomly init stars, put them around the center of the screen
		star->xpos = float(((rand() % screenWidth) * -1) + centerX);
		star->ypos = float(((rand() % screenHeight) * -1) + centerY);

		//change viewpoint
		if (mouseX > screenWidth / 2)
		{
			star->xpos *= 800.0;
			star->ypos *= 800.0;
		}
		else
		{
			star->xpos *= 40.0;
			star->ypos *= 40.0;
		}
		
		star->zpos = i;
		star->speed = (rand() % 10) + 4;
		star->color = i >> 2;
	}

void setup()
{
	//vSync = false;
	screen(960, 540, false, "stars3d");
	centerX = screenWidth / 2;
	centerY = screenHeight / 2;
	
	for (int i = 0; i < MAXSTARS; i++)
	{
		initStar(stars + i, i + 1);
	}
}

void updateAndDraw(uint32 t)
{
	clear(Color::black);

	//move and draw stars
	for (int i = 0; i < MAXSTARS; i++)
	{
		prevPosX = (stars[i].xpos / stars[i].zpos) + centerX;
		prevPosY = (stars[i].ypos / stars[i].zpos) + centerY;

		//change speed depending on mouse pos
		stars[i].speed = mouseY / 20;
		
		//move star closer
		stars[i].zpos -= stars[i].speed;
		
		//compute 3D position
		real32 tempX = (stars[i].xpos / stars[i].zpos) + centerX;
		real32 tempY = (stars[i].ypos / stars[i].zpos) + centerY;

		//check if star has moved outside of screen
		if (tempX < 0 || tempX > screenWidth - 1 || tempX < 0 || tempY > screenHeight - 1 || stars[i].zpos <= 0)
		{
			initStar(stars + i, i + 1);
			continue;
		}

		if (mouseX > screenWidth / 2)
		{
			if (tempX == prevPosX || tempY ==prevPosY)
				pixel((int)tempX, (int)tempY, 255, 255, 255);
			else
				line((int)tempX, (int)tempY, (int)prevPosX, (int)prevPosY);
		}
		else
		{
			pixel((int)tempX, (int)tempY, 0, stars[i].color, stars[i].color);
		}
	}

	uploadPixels();
}

void shutdown() { }