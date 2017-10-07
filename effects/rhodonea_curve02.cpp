	/*	Rhodonea curve (rose) plotted in polar coordinates.
		See https://en.wikipedia.org/wiki/Rose_(mathematics)
		by Martin Fairbanks
	*/
	#define SDL2
	#include "..\framework\creativeframework.cpp"
	real32 angle = 0;
	real32 k = 1;

void setup()
{
	screen(1920, 1080, true, "rhodonea curve 2");
	noStroke();
	colorMode(HSB);
}

void updateAndDraw(uint32 t)
{
	clear(150, 255, 155);
	int32 r, x, y;;
	
	k += 0.008;
	for (real32 angle = 0; angle < TWO_PI; angle += 0.01)
	{
		//radius from center
		r = windowWidth / 4 * cos(k*angle);
		x = r * cos(angle);
		y = r * sin(angle);
		fill(random(255), 255, 255);
		circle(center.x + x, center.y + y, 30);
	}

	uploadPixels();
}

void shutdown() { }