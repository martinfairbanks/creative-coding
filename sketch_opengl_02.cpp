#define OPENGL
#include "framework\creativeframework.cpp"

float xRot = 0;
float yRot = 0;
float zRot = 0;


void setup()
{
	screen(1600, 1024, false, "opengl framework");
	set3dProjection();

}

void updateAndDraw(uint32 t)
{
	if (keyDown(SDL_SCANCODE_W))
	{
		xRot -= 5.0f;
	}
	if (keyDown(SDL_SCANCODE_S))
	{
		xRot += 5.0f;
	}
	if (keyDown(SDL_SCANCODE_A))
	{
		yRot -= 5.0f;
	}
	if (keyDown(SDL_SCANCODE_D))
	{
		yRot += 5.0f;
	}
	if (keyDown(SDL_SCANCODE_Z))
	{
		zRot -= 5.5f;
	}
	if (keyDown(SDL_SCANCODE_X))
	{
		zRot += 5.5f;
	}
	
	clear(cornflowerblue);
	stroke(pink);
	
	glLoadIdentity();
	
	glTranslatef(0.0f, 0.0f, -2.0f);
	
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	cube(1.0f);
}


void shutdown() { }

