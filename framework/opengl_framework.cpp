/* Linking */
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <SDL2/SDL.h>
#include <sdl2/SDL_opengl.h>
#include <gl/glu.h>
#include <crtdbg.h> //memory leak check
#include <map>		//for keyPressed

#include "creativeframework.h"

/* Globals */
SDL_GLContext glContext;
SDL_Window *window = 0;
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

float angle = 0;						//rotation

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

void set2dProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, screenWidth, screenHeight, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
}

void set3dProjection()
{
	//set viewport to window dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	//switch to the projection matrix and reset it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//set camera perspective
	gluPerspective(45.0f,								//camera angle, field of view in degrees, set to 45 degrees viewing angle
		(GLfloat)screenWidth / (GLfloat)screenHeight,	//aspect ratio
		1.0f,											//near z clipping coordinate
		500.0f);										//far z clipping coordinate
														//1.0f - 500.0f is the start and end point for how deep we can draw into the screen

	//switch to GL_MODELVIEW, tells OGL that all future transformations will affect what we draw
	//reset the modelview matrix, wich is where the object information is stored, sets x,y,z to zero
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//enable depth buffer
	glEnable(GL_DEPTH_TEST);
}

inline void line_(int32 x0, int32 y0, int32 x1, int32 y1)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(x1, y1);
	glEnd();
}

inline void pixel_(int32 x, int32 y)
{
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

inline void rect_(int x, int y, int width, int height)
{
	if (fillFlag)
	{
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);
		glEnd();
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);
		glEnd();
	}
}

inline void circle_(int32 x, int32 y, int32 radius)
{
	if (fillFlag)
	{
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y);
		for (int angle = 0; angle < 360; angle++)
		{
			glVertex2f(x + sin(angle) * radius, y + cos(angle) * radius);
		}
		glEnd();
	}
	else
	{
		//TODO: fix this!
		glLineWidth(1);
		glBegin(GL_LINE_STRIP);
		for (float angle = 0; angle < PI32*4; angle += (PI32 / 50.0f))
		{
			glVertex2f(x + sin(angle) * radius, y + cos(angle) * radius);
		}
		glEnd();
	}
}

void screen(int width, int height, bool screen, char *title)
{
	performanceFrequency = SDL_GetPerformanceFrequency();
	srand(SDL_GetTicks());

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

	fullscreen = screen;
	int32 tempWidth = screenWidth = width;
	int32 tempHeight = screenHeight = height;

	uint32 flags = SDL_WINDOW_OPENGL;// | SDL_WINDOW_RESIZABLE;

	//set to zero to scale the window to desired resolution without changing the desktop resolution
	if (fullscreen)
	{
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;
		tempWidth = 0;
		tempHeight = 0;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, tempWidth, tempHeight, flags);
	glContext = SDL_GL_CreateContext(window);

	//use doublebuffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (vSync)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);

	set3dProjection();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void quit()
{
	SDL_GameControllerClose(controllerHandle);
	SDL_GL_DeleteContext(glContext);
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
				//Get new dimensions and repaint on window size change  
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				{
					/*screenWidth = event.window.data1;
					screenHeight = event.window.data2;
					set3dProjection();*/
				} break;

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
		//update screen
		SDL_GL_SwapWindow(window);

		uint64 endCycleCount = __rdtsc();
		uint64 endCounter = SDL_GetPerformanceCounter();
		uint64 counterElapsed = endCounter - lastCounter;
		uint64 cyclesElapsed = endCycleCount - lastCycleCount;
		real64 msPerFrame = (((1000.0f * (real64)counterElapsed) / (real64)performanceFrequency));
		real64 fps = (real64)performanceFrequency / (real64)counterElapsed;
		real64 megaCyclesPerFrame = ((real64)cyclesElapsed / (1000.0f * 1000.0f));
		lastCycleCount = endCycleCount;
		lastCounter = endCounter;

#if defined(_DEBUG)        
		char message[256];
		sprintf_s(message, "%.03fms, %.03fFPS, %.03fMEGAc/f, RefreshRate: %d\0", msPerFrame, fps, megaCyclesPerFrame, refreshRate);
		//print(message, 1, screenHeight - 20);
		SDL_SetWindowTitle(window, message);
		totalFrames++;
#endif
		
	}
#if defined(_DEBUG)   
	printf("Running time: %d seconds\n", SDL_GetTicks() / 1000);
	printf("Frames crunched: %d\n", totalFrames);
	//TODO: fix type conversions to get accurate result
	uint32 fps = (totalFrames / uint32((SDL_GetTicks() / 1000.f)));
	printf("Average FPS: %u\n", fps);
	//system("PAUSE");
#endif
	quit();
	return 0;
}

void setup()
{
	screen(960, 540, false, "opengl framework");
	//setColor(Color::magenta);
}

void updateAndDraw(uint32 t)
{
	set3dProjection();
	fill();
	angle += 1;
	//clear screen buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//reset the modelview matrix, sets x,y,z to zero
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -10.0f);
	glRotatef(angle, 0.0f, 0.0f, 0.5f);
	glScalef(1.0f, 1.0f, 1.0f);
	
	//Begin triangle coordinates
	glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(-2.0f, -2.0f, 0.0f);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(2.0f, -2.0f, 0.0f);
	glEnd();

	set2dProjection();
	glColor3f(1.0f, 0.0f, 0.0f);

	line_(50, 50, 400, 300);
	glColor3f(1.0f, 0.0f, 1.0f);
	pixel_(10, 400);
	noFill();
	circle_(screenWidth / 2, screenHeight / 2, 250);
	
	rect_(600, 30, 50, 50);

	//draw a filled rectangle
	glRectf(10.0f, 10.0f, 25.0f, 25.0f);
}


void shutdown() { }
