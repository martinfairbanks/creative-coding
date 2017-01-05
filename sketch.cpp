	//#define SDL2	
	//#define WIN_32	
	#include "framework\creativeframework.cpp"

void setup()
{
	screen(960, 540, false, "creative coding");
}

void updateAndDraw(uint32 t)
{
	clear(Color::teal);
	setColor(Color::green);
	//pixel(400, 100);
	//pixel(401, 100);
	//pixel(400, 101);
	//pixel(401, 101);
	setColor(Color::red);
	line(0, 0, 400, 400);
	fill();
	circle(200, 200, 40);
	noFill();
	setColor(Color::yellow);
	rect(400, 400, 50, 50);
	setColor(Color::magenta);
	circle(400, 400, 60);
	uploadPixels();

	//clear_(Color::teal);
	//setColor_(Color::green);
	fill();
	//rect_(100, 100, 50, 50);
	setColor_(Color::orange);
	circle_(200, 200, 10);
	setColor_(Color::orange);
	pixel_(400, 100, 0x0000ff);
	pixel_(401, 100, 0x0000ff);
	pixel_(400, 101, 0x0000ff);
	pixel_(401, 101, 0x0000ff);

	pixel_(405, 105);
	pixel_(406, 105);
	pixel_(405, 106);
	pixel_(406, 106);
	noFill();
	rect_(100, 100, 10, 10);
	setColor(Color::teal);
	line_(400, 500, 300, 200);
	//clear_(Color::green);

}

void shutdown() { }