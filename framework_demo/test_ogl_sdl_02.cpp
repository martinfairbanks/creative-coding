#define OPENGL
#include "../framework/creativeframework.cpp"

real32 xRot = 0;
real32 yRot = 0;
real32 zRot = 0;


void setup()
{
	screen(960, 540, false, "opengl framework");
	set3dProjection();
	lights();
}

void updateAndDraw()
{
	clear(cornflowerblue);
	fill(blue);

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
	
	loadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	cube(1.0f);
}


void shutdown() { }

