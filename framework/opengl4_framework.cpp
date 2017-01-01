/* Linking */
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <gl/glu.h>
#include <map>		//for keyPressed

#include "creativeframework.h"

/* Globals */
SDL_GLContext glContext;
SDL_Window *window = 0;
uint32 VBO;	//vertex buffer object id
uint32 VAO;	//vertex array object id
int32 shaderProgram;
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

/* position of each vertex point */
const char* vertexShaderSource =	"#version 430 core														\n"
									"																		\n"
									"void main(void)														\n"
									"{																		\n"
									"    gl_Position = vec4(0.0f, 0.0f, 0.5f, 1.0);							\n"
									"}																		\n";


/* color of each fragment (pixel-sized area of the triangle) */
const char* fragmentShaderSource =	"#version 430 core														\n"
									"																		\n"
									"out vec4 color;														\n"
									"																		\n"
									"void main(void)														\n"
									"{																		\n"
									"    color = vec4(0.0, 1.0, 0.0, 1.0);									\n"
									"}																		\n";

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

/*void print(char *message, int x, int y)
{
	
}*/

char* readFile(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	//get file length
	fseek(fp, 0, SEEK_END);
	long fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* contents = (char*)malloc(fileLength + 1);
	
	//clear memory
	for (int i = 0; i < fileLength + 1; i++) {
		contents[i] = 0;
	}

	fread(contents, 1, fileLength, fp);
	contents[fileLength + 1] = '\0';
	fclose(fp);
	return contents;
}

void screen(int width, int height, bool screen, char *title)
{
	performanceFrequency = SDL_GetPerformanceFrequency();
	srand(SDL_GetTicks());

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

	fullscreen = screen;
	int32 tempWidth = screenWidth = width;
	int32 tempHeight = screenHeight = height;

	uint32 flags = SDL_WINDOW_OPENGL;

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

	//initialize GLEW
	glewInit();

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	if (!glewIsSupported("GL_VERSION_3_3")) {
		LOGERROR("OpenGL 3.3 not available");
	}

	LOG(glGetString(GL_VERSION));

	/* build and compile shader program */
	//const char *vertexShader = readFile("data/shaders/vertexshader2.vert");
	//const char *fragmentShader = readFile("data/shaders/fragmentshader2.frag");

	//vertex shader
	int32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//check for compilation errors
	int32 success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		LOG("Vertex shader error: " << infoLog);
	}

	//fragment shader
	int32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//check for compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		LOG("Fragment shader error: " << infoLog);
	}

	//link shaders to make the shaderprogram
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*real32 vertices[] = {
		//first triangle
		1.0f,  1.0f, 0.0f,	//top Right
		1.0f, -1.0f, 0.0f,	//bottom Right
		-1.0f,  1.0f, 0.0f, //top Left 
		//second triangle
		1.0f, -1.0f, 0.0f,	//bottom Right
		-1.0f, -1.0f, 0.0f, //bottom Left
		-1.0f,  1.0f, 0.0f  //top Left
	};*/
	
	// Set up vertex data (and buffer(s)) and attribute pointers
	real32 vertices[] = {
		-0.5f, -0.5f, 0.0f, // Left  
		0.5f, -0.5f, 0.0f, // Right 
		0.0f, 0.5f, 0.0f  // Top   
	};

	//generate 1 vertex buffer object
	glGenBuffers(1, &VBO);
	//buffer calls to the GL_ARRAY_BUFFER targets the currently bound buffer (VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copies the vertex data into the buffer’s memory
	//GL_STREAM_DRAW - the data will change every time it is drawn
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//position attribute pointer - tells opengl how our data buffer is structured
	glVertexAttribPointer(0,	//index of vertex attribute to modify
		3,						//size of the vertex attribute, number of components. The vertex attribute is a vec3 so it is composed of 3 values.
		GL_FLOAT,				//data type of each component in the array
		GL_FALSE,				//normalized = false
		3 * sizeof(real32),	//stride, space between consecutive vertex attribute sets
		(void*)0);			//offset, where to start read in the buffer
	glEnableVertexAttribArray(0);
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
		//print(message, 1, screenHeight - 20);
		SDL_SetWindowTitle(window, message);
		totalFrames++;
#endif
		//update screen
		SDL_GL_SwapWindow(window);

		lastCycleCount = endCycleCount;
		lastCounter = endCounter;
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
}

void updateAndDraw(uint32 t)
{
	/*const GLfloat color[] = { (float)sin(SDL_GetTicks() / 1000), 0.5f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);
	glUseProgram(shaderProgram);
	glPointSize(40.0f);
	glDrawArrays(GL_POINTS, 0, 1);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	*/

	const real32 color[] = { 0.2f, 0.3f, 0.3f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);

	//set shader program
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}


void shutdown() { }
