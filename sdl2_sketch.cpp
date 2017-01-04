	#include "framework\creativeframework.cpp"

void setup()
{
	screen(960, 540, false, "creative coding");

}

void updateAndDraw(uint32 t)
{
	clear(Color::teal);
	setColor(Color::green);
	pixel(400, 100);
	pixel(401, 100);
	pixel(400, 101);
	pixel(401, 101);
	setColor(Color::red);
	line(0, 0, 200, 200);
	fill();
	circle(200, 200, 40);
	noFill();
	setColor(Color::magenta);
	circle(400, 400, 60);
	uploadPixels();

	setColor_(Color::green);
	fill();
	rect_(100, 100, 50, 50);
	circle_(200, 200, 50);
	setColor_(Color::yellow);
	pixel_(400, 400);
	line_(400, 500, 300, 200);
	
}


void shutdown() { }