/* Linking */
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_ttf.lib")

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>	//sprintf
#include <map>		//for keyPressed

#define SDL2
#include "creativeframework.h"

/* Globals */
SDL_Renderer *renderer = 0;
SDL_Window *window = 0;
TTF_Font* font = 0;
const Uint8 *keystates = 0;
std::map<int, bool> keyArray;
SDL_GameController *controllerHandle;

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
const int32 joyDeadZone = 8000;

SDL_Texture *backbufferTexture;	//texture used for drawing with the pixelbuffer

void setup();
void updateAndDraw(uint32 t);

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
	else 
		keyArray[key] = false;

	return false;
}

inline void uploadPixels()
{
	SDL_UpdateTexture(backbufferTexture, NULL, pixelBuffer, screenWidth * sizeof(uint32));
	SDL_RenderCopy(renderer, backbufferTexture, NULL, NULL);
	//SDL_RenderPresent(renderer);
};

inline void flip()
{
	SDL_RenderPresent(renderer);
};

/* SDL hardware rendering */
inline void _setColor(int32 r, int32 g, int32 b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
}

inline void _setColor(ColorRGB col)
{
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, 0xff);
}

inline void _pixel(int32 x, int32 y)
{
	SDL_RenderDrawPoint(renderer, x, y);
}

inline void _line(int32 x0, int32 y0, int32 x1, int32 y1)
{
	SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}

inline void _circle(int32 x0, int32 y0, int32 radius)
{
	if (fillFlag)
	{
		int32 x = radius;
		int32 y = 0;
		int32 err = 0;

		while (x >= y)
		{
			SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
			SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
			SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
			SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
			SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
			SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
			SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
			SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);

			y += 1;
			err += 1 + 2 * y;
			if (2 * (err - x) + 1 > 0)
			{
				x -= 1;
				err += 1 - 2 * x;
			}
		}
	}
	else
	{
		int32 r2 = radius * radius;
		int32 area = r2 << 2;
		int32 rr = radius << 1;

		for (int32 i = 0; i < area; i++)
		{
			int32 tx = (i % rr) - radius;
			int32 ty = (i / rr) - radius;

			if (tx * tx + ty * ty <= r2)
				SDL_RenderDrawPoint(renderer, x0 + tx, y0 + ty);
		}
	}
}

inline void _rect(int xPos, int yPos, int width, int height)
{
	SDL_Rect rect = { xPos, yPos, width, height };
	if (fillFlag)
		SDL_RenderFillRect(renderer, &rect);
	else
		SDL_RenderDrawRect(renderer, &rect);
}


void print(char *message, int32 x, int32 y)
{
	SDL_Surface *surf = TTF_RenderText_Solid/*TTF_RenderText_Blended*/(font, message, SDL_Color{ (uint8)color.r, (uint8)color.g, (uint8)color.b });
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);

	//get width and height of texture
	int32 w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	SDL_Rect dst = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_DestroyTexture(texture);
}

void screen(int32 width, int32 height, bool32 screen, char *title)
{
	performanceFrequency = SDL_GetPerformanceFrequency();
	srand(SDL_GetTicks());

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

	fullscreen = screen;
	int32 tempWidth = screenWidth = width;
	int32 tempHeight = screenHeight = height;

	uint32 flags = SDL_WINDOW_RESIZABLE; //SDL_WINDOW_SHOWN

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

	backbufferTexture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		screenWidth, screenHeight);

	//pixelBuffer = new unsigned int[screenWidth * screenHeight];	
	pixelBuffer = (uint32 *)malloc(screenWidth * screenHeight * 4);
	//pixelBuffer = (uint32 *)VirtualAlloc(0, screenWidth * screenHeight * bytesPerPixel, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	TTF_Init();
	font = TTF_OpenFont("data/verdana.ttf", 12);

	controllerHandle = SDL_GameControllerOpen(0);
	SDL_GetMouseState(&mouseX, &mouseY);

	for (int32 i = 1; i <= 3; i++)
	{
		mouseButton[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
	}

	srand(SDL_GetTicks());
	
	SDL_SetRenderDrawColor(renderer, 0x65, 0x9C, 0xEF, 255);
	SDL_RenderClear(renderer);
}

void quit()
{
	free(pixelBuffer);
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

	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "", "", NULL);
	setup();

	const real64 MAX_FRAME_TIME = 1000 / 60;
	int32 refreshRate = getWindowRefreshRate();
	uint32 frameStart = SDL_GetTicks(); //LAST_UPDATE_TIME
	uint32 totalFrames = 0;
	uint64 lastCounter = SDL_GetPerformanceCounter();
	uint64 lastCycleCount = __rdtsc();
	SDL_Event event;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
				{
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
					} break;
				} break;

				case SDL_KEYUP:
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
						running = false;
				} break;

				case SDL_QUIT:
				{
					running = false;
				} break;
			}
		}
		
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
		
		/* the time it takes to draw one frame */
		uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameTime > MAX_FRAME_TIME)
			frameTime = (uint32)MAX_FRAME_TIME;
		frameStart = SDL_GetTicks();

		updateAndDraw(frameTime);

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
		totalFrames++;
#endif
		//update screen
		//uploadPixels();
		SDL_RenderPresent(renderer);

		lastCycleCount = endCycleCount;
		lastCounter = endCounter;
	}
#if defined(_DEBUG)   
	printf("Running time: %d seconds\n", SDL_GetTicks() / 1000);
	printf("Frames crunched: %d\n", totalFrames);
	//TODO: fix type conversions to get a more accurate result
	uint32 fps = (totalFrames / uint32((SDL_GetTicks() / 1000.f)));
	printf("Average FPS: %u\n", fps);
	//system("PAUSE");
#endif
	quit();
	return 0;
}

void setup()
{
	screen(960, 540, false, "creative coding");

}

void updateAndDraw(uint32 t)
{
	/*static int x = 400;

	if (joyStickX < -joyDeadZone || keyPressed(SDL_SCANCODE_D))
		x++;

	if (joyStickX > joyDeadZone || keyPressed(SDL_SCANCODE_A))
		x--;

	SDL_SetRenderDrawColor(renderer, 0x65, 0x9C, 0xEF, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawPoint(renderer, x, 240);*/

	clear(0x659CEF);
	clear(Color::cyan);
	clear(0xff, 0xff, 0xff);
	clear(0x659CEF);
	color = Color::red;
	pixel(400, 100);
	pixel(401, 100);
	pixel(400, 101);
	pixel(401, 101);
	line(0, 0, 200, 200);
	circle(200, 200, 40);
	noFill();
	color = Color::magenta;
	circle(400, 400, 60);
	uploadPixels();

	_setColor(Color::green);
	fill();
	_rect(100, 100, 50, 50);
	_circle(200, 200, 50);
	_setColor(Color::yellow);
	_pixel(400, 400);
	_line(400, 500, 300, 200);

}


void shutdown() { }
