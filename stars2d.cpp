	//#define SDL2	
	//#define WIN_32	
	#include "framework\creativeframework.cpp"

	#define MAXSTARS 600

	struct Star
	{											
		real32 x, y;							
		uint8 plane;							
	} *stars;

	enum Direction
	{
		STARS_LEFT,
		STARS_RIGHT
	};

	Direction direction = STARS_RIGHT;
	unsigned int starColors[3] = { 0x30, 0x80, 0xff };
	float speed = 0.6f;

void setup()
{
	//vSync = false;
	screen(960, 540, false, "stars2d");
	//allocate memory
	stars = new Star[MAXSTARS];

	//randomly generate some stars
	for (int i = 0; i < MAXSTARS; i++) {
		stars[i].x = float(rand() % screenWidth);
		stars[i].y = float(rand() % screenHeight);
		stars[i].plane = rand() % 3; //star colour between 0 and 2
	}
}

void updateAndDraw(uint32 t)
{
	if (mouseX > screenWidth / 2)
		direction = STARS_RIGHT;
	else
		direction = STARS_LEFT;

	clear(Color::black);
	
	speed = (float)mouseY / 80.0f;

	if (direction == STARS_RIGHT)
	{
		//loop through stars and paint them one by one
		for (int i = 0; i<MAXSTARS; i++) {

			//move star with a speed depending on plane
			stars[i].x += (1.0f + (float)stars[i].plane)*speed;
		
			//check if star has moved outside of screen
			if (stars[i].x>screenWidth)
			{
				stars[i].x = 0;

				//new random y pos
				stars[i].x = float((rand() % 100) * -1.0f);	//to prevent stars from lining up after fast speed
				stars[i].y = float(rand() % screenHeight);
				//LOG(stars[i].x);
			}
			//set color and paint star
			setColor(0, starColors[stars[i].plane], 0);
			pixel((int)stars[i].x, (int)stars[i].y);
		}
	}
	else
	{
		for (int i = 0; i<MAXSTARS; i++) {

			stars[i].x -= (1 + (float)stars[i].plane)*speed;

			if (stars[i].x <= 0)
			{
				stars[i].x = float((rand() % 100) + screenWidth);
				stars[i].y = float(rand() % screenHeight);
			}
			setColor(0, 0, starColors[stars[i].plane]);
			pixel((int)stars[i].x, (int)stars[i].y);
			pixel((int)stars[i].x + 1, (int)stars[i].y);
		}
	}

	uploadPixels();
}

void shutdown() 
{
	delete stars;
}