	/*	Rhodonea curve (rose) plotted in polar coordinates.
		See https://en.wikipedia.org/wiki/Rose_(mathematics)
		by Martin Fairbanks
	*/

	#define SDL2
	#include "..\framework\creativeframework.cpp"
	int32 time;
	real32 k = 1, c = 1, n = 1, d = 1;

void setup()
{
	screen(1920, 1080, true, "rhodonea curve 2");
	clear(teal);
	noStroke();
	time = SDL_GetTicks();
}

void updateAndDraw(uint32 t)
{
	int32 radius, x, y;
	
	k = n / d;
	for (real32 angle = 0; angle < TWO_PI * 10; angle += 0.02)
	{
		radius = windowWidth / 4 * cos(k * angle);
		x = radius * cos(angle);
		y = radius * sin(angle);
		circle(center.x + x, center.y + y, 7);
	}

	int32 dt = SDL_GetTicks();

	//switch pattern every second
	if ((dt - time) > 1000)
	{
		d = randomf(1.0f, 10.0f);
		n = randomf(1.0f, 10.0f);
		fill(random(255), random(255), random(255));
		clear(teal);
		time = SDL_GetTicks();
	}

	uploadPixels();
}

void shutdown() { }