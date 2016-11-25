/* Linking */
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_ttf.lib")

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h> //srand
#include <crtdbg.h> //memory leak check
#include <sstream>	//sprintf
#include <map>		//for keyPressed

SDL_Renderer *renderer = 0;
SDL_Window *window = 0;
TTF_Font* font = 0;

struct RGBColor
{
	int32 r;
	int32 g;
	int32 b;
} color{ 0xff,0xff,0xff };

void setup();
void updateAndDraw(uint32 t);

/* Input */
const Uint8 *keystates = 0;
std::map<int, bool> keyArray;
int32 mouseX = 0;
int32 mouseY = 0;

enum MouseButtons
{
	LEFT = 1,
	MIDDLE = 2,
	RIGHT = 3
};

SDL_GameController *controllerHandle;

uint32 mouseButton[3];
uint8 joyUp;
uint8 joyDown;
uint8 joyLeft;
uint8 joyRight;
uint8 joyStart;
uint8 joyBack;
uint8 joyLeftShoulder;
uint8 joyRightShoulder;
uint8 joyAButton;
uint8 joyBButton;
uint8 joyXButton;
uint8 joyYButton;

int32 joyStickX;
int32 joyStickY;
int32 joyRightStickX;
int32 joyRightStickY;
static const int32 joyDeadZone = 8000;

bool keyDown(int32 key)
{
	//returns true until keyup
	return (keystates[key] != 0);
}

bool keyPressed(int32 key)
{
	//returns true once, until keyup
	if (keyArray.find(key) == keyArray.end())
		keyArray[key] = false;

	if (keystates[key])
	{
		if (keyArray[key] == false)
		{
			keyArray[key] = true;
			return true;
		}
	}
	else keyArray[key] = false;
	return false;
}

inline void updateInput()
{
	if (SDL_GameControllerGetAttached(controllerHandle))
	{
		joyUp = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_DPAD_UP);
		joyDown = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		joyLeft = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		joyRight = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
		joyStart = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_START);
		joyBack = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_BACK);
		joyLeftShoulder = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
		joyRightShoulder = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
		joyAButton = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_A);
		joyBButton = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_B);
		joyXButton = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_X);
		joyYButton = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_Y);

		joyStickX = SDL_GameControllerGetAxis(controllerHandle, SDL_CONTROLLER_AXIS_LEFTX);
		joyStickY = SDL_GameControllerGetAxis(controllerHandle, SDL_CONTROLLER_AXIS_LEFTY);
		joyRightStickX = SDL_GameControllerGetAxis(controllerHandle, SDL_CONTROLLER_AXIS_RIGHTX);
		joyRightStickY = SDL_GameControllerGetAxis(controllerHandle, SDL_CONTROLLER_AXIS_RIGHTY);
	}

	keystates = SDL_GetKeyboardState(NULL);
	SDL_GetMouseState(&mouseX, &mouseY);

	for (int i = 1; i <= 3; i++)
	{
		mouseButton[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
	}
}

void print(char *message, int x, int y)
{
	SDL_Surface *surf = TTF_RenderText_Solid/*TTF_RenderText_Blended*/(font, message, SDL_Color{ (uint8)color.r, (uint8)color.g, (uint8)color.b });
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);

	//get width and height of texture
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	SDL_Rect dst = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_DestroyTexture(texture);
}

void screen(int width, int height, bool screen, char *title)
{
	performanceFrequency = SDL_GetPerformanceFrequency();

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

	fullscreen = screen;
	int32 tempWidth = screenWidth = width;
	int32 tempHeight = screenHeight = height;

	uint32 flags = SDL_WINDOW_SHOWN;

	//set to zero to scale the window to desired resolution without changing the desktop resolution
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
		tempWidth = 0;
		tempHeight = 0;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, tempWidth, tempHeight, flags);
	if (vSync)
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	else
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (fullscreen)
	{
		//scale window to desired resolution
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);
	}

	TTF_Init();
	font = TTF_OpenFont("data/verdana.ttf", 12);

	controllerHandle = SDL_GameControllerOpen(0);
	SDL_GetMouseState(&mouseX, &mouseY);

	for (int i = 1; i <= 3; i++)
	{
		mouseButton[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
	}
	srand(SDL_GetTicks());

	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing file", "File is missing. Please reinstall the program.", NULL);

	SDL_SetRenderDrawColor(renderer, 0x65, 0x9C, 0xEF, 255);
	SDL_RenderClear(renderer);
}

void quit()
{
	SDL_GameControllerClose(controllerHandle);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}

int getWindowRefreshRate()
{
	//get refresh rate
	SDL_DisplayMode mode;
	int32 defaultRefreshRate = 60;

	//if we can't get the rate return the deafult one
	if (SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(window), &mode) != 0)
		return defaultRefreshRate;

	if (mode.refresh_rate == 0)
		return defaultRefreshRate;

	return mode.refresh_rate;
}

int main(int argc, char** argv)
{
	//Check for memory leak if debug build
#if defined(_DEBUG)        
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif	

	setup();

	int32 refreshRate = getWindowRefreshRate();

	unsigned long long lastCounter = SDL_GetPerformanceCounter();
	unsigned long long lastCycleCount = __rdtsc();

	while (running)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_RETURN:
					{
						if (fullscreen)
						{
							SDL_SetWindowFullscreen(window, SDL_FALSE);
							fullscreen = false;
						}
						else
						{
							SDL_SetWindowFullscreen(window, SDL_TRUE);
							fullscreen = true;
						}
					}
				}
				break;

			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					running = false;
				break;
			case SDL_QUIT:
				running = false;
				break;
			}
		}
		updateInput();
		time = SDL_GetTicks();
		
		SDL_SetRenderDrawColor(renderer, 0x65, 0x9C, 0xEF, 255);
		SDL_RenderClear(renderer);

		updateAndDraw(time);

		uint64 endCycleCount = __rdtsc();
		uint64 endCounter = SDL_GetPerformanceCounter();
		uint64 counterElapsed = endCounter - lastCounter;
		uint64 cyclesElapsed = endCycleCount - lastCycleCount;

		real64 msPerFrame = (((1000.0f * (real64)counterElapsed) / (real64)performanceFrequency));
		real64 fps = (real64)performanceFrequency / (real64)counterElapsed;
		real64 megaCyclesPerFrame = ((real64)cyclesElapsed / (1000.0f * 1000.0f));

#if defined(_DEBUG)        
		char message[256];
		sprintf_s(message, "%.03fms, %.03fFPS, %.03fMEGAc/f, RefreshRate: %d\0", msPerFrame, fps, megaCyclesPerFrame, refreshRate);
		print(message, 1, screenHeight - 20);
		SDL_SetWindowTitle(window, message);
#endif
		//update screen
		SDL_RenderPresent(renderer);

		lastCycleCount = endCycleCount;
		lastCounter = endCounter;
	}
	quit();
	return 0;
}
