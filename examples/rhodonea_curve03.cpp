	/*	Rhodonea curve (rose) plotted in polar coordinates.
		See https://en.wikipedia.org/wiki/Rose_(mathematics)
		by Martin Fairbanks
	*/
	#define SDL2
	#include "..\framework\creativeframework.cpp"
	real32 k = 1.f;
	real32 c = 1.f;
	int32 time;
	
void setup()
{
	screen(1920, 1080, true, "rhodonea curve 3");
	noStroke();
	time = SDL_GetTicks();
	clear(0);
	colorMode(HSB);
}

void updateAndDraw()
{
	int32 x, y;;
	real32 r;
	c += 0.8f;
	k = 7.f;
	for (real32 angle = 0.f; angle < TWO_PI; angle += 0.005f)
	{
		//radius from center + offset
		r = 400 * cos(k*angle) + c;
		x = (int32)(r * cos(angle));
		y = (int32)(r * sin(angle));
		//fill(random(255), random(255), random(255));
		circle(center.x + x, center.y + y, 5.f);
	}

	int32 dt = SDL_GetTicks();
	
	if ((dt - time) > 500)
	{
		fill(random(255), 255, 255);
		time = SDL_GetTicks();
	}
}

void shutdown() { }