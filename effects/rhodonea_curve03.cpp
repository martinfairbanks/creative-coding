	/*	Rhodonea curve (rose) plotted in polar coordinates.
		See https://en.wikipedia.org/wiki/Rose_(mathematics)
		by Martin Fairbanks
	*/
	#define SDL2
	#include "..\framework\creativeframework.cpp"
	real32 angle = 0;
	real32 k = 1;
	real32 c = 1;
	int32 time;
	
void setup()
{
	screen(1920, 1080, true, "rhodonea curve 3");
	noStroke();
	time = SDL_GetTicks();
	clear(0);
	colorMode(HSB);
}

void updateAndDraw(uint32 t)
{
	int32 r, x, y;
	
	c += 0.8;
	k = 7;
	for (real32 angle = 0; angle < TWO_PI; angle += 0.005)
	{
		//radius from center + offset
		r = 400 * cos(k*angle) + c;
		x = r * cos(angle);
		y = r * sin(angle);
		//fill(random(255), random(255), random(255));
		circle(center.x + x, center.y + y, 5);
	}

	int32 dt = SDL_GetTicks();
	
	if ((dt - time) > 500)
	{
		fill(random(255), 255, 255);
		time = SDL_GetTicks();
	}

	uploadPixels();
}

void shutdown() { }