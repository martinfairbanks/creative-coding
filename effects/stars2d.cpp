/*	2D starfield
	by Martin Fairbanks
*/
	#define SDL2
	#include "..\framework\creativeframework.cpp"
	#define MAXSTARS 600

	struct Star
	{											
		float32 x, y;							
		uint8 colorPlane;							
	} stars[MAXSTARS];

	enum Direction
	{
		STARS_LEFT,
		STARS_RIGHT
	};

	Direction direction = STARS_RIGHT;
	unsigned int starColors[3] = { 0x30, 0x80, 0xff };
	float32 speed = 0.6f;

void setup()
{
	//vSync = false;
	screen(960, 540, true, "stars2d");

	//randomly generate some stars
	for (int i = 0; i < MAXSTARS; i++) {
		stars[i].x = float(rand() % screenWidth);
		stars[i].y = float(rand() % screenHeight);
		stars[i].colorPlane = rand() % 3; //star color between 0 and 2
	}
}

void updateAndDraw(uint32 t)
{
	if (mouseX > screenWidth / 2)
		direction = STARS_RIGHT;
	else
		direction = STARS_LEFT;

	clear(black);
	
	speed = (float)mouseY / 80.0f;

	if (direction == STARS_RIGHT)
	{
		//move and draw stars
		for (int i = 0; i < MAXSTARS; i++) {

			//move star with a speed depending on plane
			stars[i].x += (1.0f + (float)stars[i].colorPlane)*speed;
		
			//check if star has moved outside of screen
			if (stars[i].x > screenWidth)
			{
				stars[i].x = 0;

				//new random y pos
				stars[i].x = float((rand() % 100) * -1.0f);	//to prevent stars from lining up after fast speed
				stars[i].y = float(rand() % screenHeight);
			}
			//set color and paint star
			stroke(0, starColors[stars[i].colorPlane], 0);
			pixel((int)stars[i].x, (int)stars[i].y);
		}
	}
	else
	{
		for (int i = 0; i < MAXSTARS; i++) {

			stars[i].x -= (1 + (float)stars[i].colorPlane)*speed;

			if (stars[i].x <= 0)
			{
				stars[i].x = float((rand() % 100) + screenWidth);
				stars[i].y = float(rand() % screenHeight);
			}
			stroke(0, 0, starColors[stars[i].colorPlane]);
			pixel((int)stars[i].x, (int)stars[i].y);
			pixel((int)stars[i].x + 1, (int)stars[i].y);
		}
	}

	uploadPixels();
}

void shutdown() { }