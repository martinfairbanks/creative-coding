	#define SDL2
	#include "..\framework\creativeframework.cpp"
	real32 angle = 0;

void setup()
{
	screen(1920, 1080, true, "pixel buffer");
	clear(teal); 
	noStroke();
	colorMode(HSB);
}

void updateAndDraw(uint32 t)
{
	int32 x = mapVal(sin(angle)*sin(angle*0.8), -1, 1, 30, windowWidth-30);
	int32 y = mapVal(sin(angle*1.1 + 2.5)*sin(angle*3.1+ 2.3), -1, 1, 30, windowHeight-30);
	real32 hue = mapVal(sin(angle), -1, 1, 0, 255);
	real32 saturation = mapVal(sin(angle), -1, 1, 127, 255);
	real32 brightness = mapVal(sin(angle*1.3)*sin(angle*2.5), -1, 1, 10, 200);
	real32 size = mapVal(sin(angle*2.5)*sin(angle*0.3), -1, 1, 5, 30);
	real32 alpha = mapVal(sin(angle*0.5), -1, 1, 50, 255);

	fill(hue, saturation, brightness, alpha);
	circle(x, y, size);

	angle = angle + 0.03;

	uploadPixels();
}

void shutdown() { }