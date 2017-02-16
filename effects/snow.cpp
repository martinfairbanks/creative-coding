/*	Snowflakes
	by Martin Fairbanks
*/
	#define SDL2
	#include "framework\creativeframework.cpp"
	#define MAX_FLAKES	2000		
	#define MAX_LAYERS	5

	struct SnowFlake
	{
		int32 x, y;
		int32 layer;
	} snow[MAX_FLAKES];

void setup()
{
	//vSync = false;
	screen(960, 540, false, "snow");

	//init snowflakes
	for (int i = 0; i < MAX_FLAKES; i++) {
		snow[i].x = rand() % screenWidth;
		snow[i].y = rand() % screenHeight;
		snow[i].layer = rand() % MAX_LAYERS;
	}
}

void updateAndDraw(uint32 t)
{
	clear(40,20,220);

	for (int i = 0; i < MAX_FLAKES; i++)
	{
		//move snowflake down depending on layer
		snow[i].y += snow[i].layer + 1;

		//check if snowflake moved outside of screen
		if (snow[i].y > screenHeight)
		{
			snow[i].x = rand() % screenWidth;
			snow[i].y = 0;
			snow[i].layer = rand() % MAX_LAYERS;
		}

		//shake and draw
		snow[i].x = (snow[i].x + (rand() % 5) - 2);
		pixel((int)snow[i].x, (int)snow[i].y, 0xffffff);
	}

	uploadPixels();
}

void shutdown() { }