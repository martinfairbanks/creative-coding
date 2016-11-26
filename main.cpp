//build as a single translation unit
#include "framework/sdl2_framework.cpp"

void setup()
{
	screen(960, 540, false, "creative coding");

}

void updateAndDraw(uint32 t)
{
	static int x = 400;

	if (joyStickX < -joyDeadZone || keyPressed(SDL_SCANCODE_D))
		x++;

	if (joyStickX > joyDeadZone || keyPressed(SDL_SCANCODE_A))
		x--;

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawPoint(renderer, x, 240);
}


void shutdown() { }
