#define OPENGL
#include "framework\creativeframework.cpp"
	
float angle = 0;
ColorRGBA col;
vec2 spot;
int32 x;
int32 y;

void setup()
{
	screen(960, 540, false, "opengl framework");
	set2dProjection();
	clear(0);
	stroke(blue);
}

void updateAndDraw(uint32 t)
{
	setLineWidth(2);
	stroke(blue);
	fill(green);
	line(50, 50, 400, 300);
	circle(screenWidth / 2, screenHeight / 2, 250);
	fill(1.0f,0,0,0.5f);
	circle(x, y, 10);
	rect(600, 30, 50, 50);
	stroke(pink);
	fill(magenta);
	ellipse(300, 400, 80, 100);
	stroke(red);
	pixel(screenWidth / 2, screenHeight / 2);
	noFill();
	circle(10, 40, 50);
	rect(20, 60, 50, 50);
}


void shutdown() { }

