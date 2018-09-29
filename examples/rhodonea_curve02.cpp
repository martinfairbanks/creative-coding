	/*	Rhodonea curve (rose) plotted in polar coordinates.
		See https://en.wikipedia.org/wiki/Rose_(mathematics)
		by Martin Fairbanks
	*/
	#define SDL2
	#include "..\framework\creativeframework.cpp"
	real32 k = 1.f;

void setup()
{
	screen(1920, 1080, true, "rhodonea curve 2");
	noStroke();
	colorMode(HSB);
}

void updateAndDraw()
{
	clear(150, 255, 155);
	int32 x, y;;
	real32 r;
	k += 0.008f;
	for (real32 angle = 0.f; angle < TWO_PI; angle += 0.01f)
	{
		//radius from center
		r = windowWidth / 4 * cos(k*angle);
		x = (int32)(r * cos(angle));
		y = (int32)(r * sin(angle));
		fill(random(255), 255, 255);
		circle(center.x + x, center.y + y, 30.f);
	}
}

void shutdown() { }