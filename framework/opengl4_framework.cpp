	/* Linking */
	#pragma comment(lib, "SDL2.lib")
	#pragma comment(lib, "SDL2main.lib")
	#pragma comment(lib, "opengl32.lib")
	//#pragma comment(lib, "glu32.lib")
	#pragma comment(lib, "glew32.lib")
	#pragma comment(lib, "SOIL.lib")
	
	/* Includes */
	#include <SDL2/SDL.h>
	#include <GL/glew.h>
	#include <gl/glu.h>
	#include <map>		//for keyPressed
	#include <SOIL.h>

	/* Globals */
	SDL_GLContext glContext;
	SDL_Window *window = 0;
	SDL_GameController *controllerHandle;
	const uint8 *keystates = 0;
	std::map<int, bool> keyArray;
	uint32 globalTime;
	//uint32 vbo;	//vertex buffer object id
	uint32 vao;	//vertex array object id
	const char *vertexShaderSource = 0;
	const char *fragmentShaderSource = 0;

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
void clear(float32 r, float32 g, float32 b, float32 a = 1.f)
{
	const float32 col[] = { r, g, b, a };
	glClearBufferfv(GL_COLOR, 0, col);
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
}

void clear(ColorRGBA col)
{
	const float32 c[] = { col.r, col.g, col.b, col.a };
	glClearBufferfv(GL_COLOR, 0, c);
}

char* readFile(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	//move pointer to the end of the file to get length
	fseek(fp, 0, SEEK_END);
	long file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *contents = (char *)malloc(file_length + 1);
	//clear memory
	for (int i = 0; i < file_length + 1; i++) {
		contents[i] = 0;
	}
	//read file
	fread(contents, 1, file_length, fp);
	//insert null terminator
	contents[file_length + 1] = '\0';
	fclose(fp);
	return contents;
}

struct Shader
{
	//how many attributes to use ->0 = position, 1 = color etc.
	GLuint numAttributes = 0;
	GLuint programID = 0;
	GLuint vertexShaderID = 0;
	GLuint fragmentShaderID = 0;

	Shader() {};

	void loadShader(const char *vertexShaderSource, const char *fragmentShaderSource)
	{
		programID = glCreateProgram();

		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (vertexShaderID == 0)
		{
			LOGERROR("Vertex shader failed to be created!");
		}

		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (fragmentShaderID == 0)
		{
			LOGERROR("Fragment shader failed to be created!");
		}

		compileShader(vertexShaderSource, vertexShaderID);
		compileShader(fragmentShaderSource, fragmentShaderID);
	}

	void linkShaders()
	{
		//link shaders to make the shaderprogram
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		//check for linking errors
		GLint success = 0;
		char log[512];
		glGetProgramiv(programID, GL_LINK_STATUS, (int *)&success);
		if (!success)
		{
			glGetProgramInfoLog(programID, 512, NULL, log);
			glDeleteProgram(programID);
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
			LOGERROR("Error when linking shaders:\n" << log);
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	//adds a attribute to the shader - what we want the shader to process, position, colors, textures
	void addAttribute(char *attributeName)
	{
		glBindAttribLocation(programID, numAttributes, attributeName);
		numAttributes++;
	}

	GLint getUniformLocation(char* uniformName)
	{
		GLint location = glGetUniformLocation(programID, uniformName);

		if (location == GL_INVALID_INDEX)
		{
			LOGERROR("Uniform " << uniformName << " not found in shader!");
		}
		return location;
	}


	void enable()
	{
		glUseProgram(programID);
		//enable attributes
		for (int i = 0; i < numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void disable()
	{
		glUseProgram(0);
		for (int i = 0; i < numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}
	
	void compileShader(const char *filename, GLuint id)
	{
		const char *contents = readFile(filename);// fileContents.c_str();
		glShaderSource(id, 1, &contents, 0);
		glCompileShader(id);

		//check for compilation errors
		int32 success = 0;
		char log[512];
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, log);
			glDeleteShader(id);
			LOGERROR("Shader " << filename << " error:\n" << log);
		}
	}
};

Shader colorShader, textureShader;

void setColor(float32 r, float32 g, float32 b, float32 a = 1.f)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	glUseProgram(colorShader.programID);
	GLfloat c[] = { r, g, b, a };
	// Update the value of input attribute 0
	glVertexAttrib4fv(1, c);
}

void setColor(ColorRGBA col)
{
	color = col;
	glUseProgram(colorShader.programID);
	GLfloat c[] = { col.r, col.g, col.b, col.a };
	// Update the value of input attribute 0
	glVertexAttrib4fv(1, c);
}

void fill()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void noFill()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

	//define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	LOG("OpenGL version: " << glGetString(GL_VERSION));
	
	if (!glewIsSupported("GL_VERSION_4_5"))
	{
		printf("OpenGL 4.5 is not available!\n");
	}

	/* build and compile shader program */
	
	colorShader.loadShader("data/shaders/colorshader.vert", "data/shaders/colorshader.frag");
	colorShader.linkShaders();

	textureShader.loadShader("data/shaders/textureshader.vert", "data/shaders/textureshader.frag");
	textureShader.linkShaders();

	/*float32 vertices[] = {
		//first triangle
		1.0f,  1.0f, 0.0f,	//top Right
		1.0f, -1.0f, 0.0f,	//bottom Right
		-1.0f,  1.0f, 0.0f, //top Left 
		//second triangle
		1.0f, -1.0f, 0.0f,	//bottom Right
		-1.0f, -1.0f, 0.0f, //bottom Left
		-1.0f,  1.0f, 0.0f  //top Left
	};*/
	
	/*
	float32 vertices[] =
	{
		-1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
	};

	GLuint vbo, ebo;

	//generate 1 vertex buffer object
	glGenBuffers(1, &vbo);
	//bind the buffer to the GL_ARRAY_BUFFER binding point
	//buffer calls to the GL_ARRAY_BUFFER targets the currently bound buffer (vbo)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//allocate memory and copies the vertex data into the buffer that is bound to the GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//enable the position attribute - enable the data that was copied to the buffer
	//to go to through the graphics processing pipeline 
	glEnableVertexAttribArray(0);

	//tells opengl how our data buffer is structured - point it to the start of the buffer
	glVertexAttribPointer(0,	//index of vertex attribute to modify
		2,						//size of the vertex attribute, number of components. The vertex attribute is a vec2 so it is composed of 2 values.
		GL_FLOAT,				//data type of each component in the array
		GL_FALSE,				//normalized = false
		0,						//stride, space between consecutive vertex attribute sets
		0);						//offset, where to start read in the buffer
	*/

	/***** Texture *****/

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		//positions          //colors           //texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,	//top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,	//bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,	//bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f	//top left 
	};

	GLuint indices[] = {
		0, 1, 3, //first triangle
		1, 2, 3  //second triangle
	};

	GLuint vbo,  ebo;
	//generate a vertex array object that stores the vbo, ebo and the calls to glEnableVertexAttribArray and glVertexAttribPointer
	glGenVertexArrays(1, &vao);
	//generate 1 vertex buffer
	glGenBuffers(1, &vbo);
	//generate one element buffer object, that stores indices that OpenGL uses to decide what vertices to draw
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//texcoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	//unbind vao
	glBindVertexArray(0);

	// Load and create a texture 
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int textureWidth, textureHeight;
	unsigned char* image = SOIL_load_image("data/container.jpg", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

void quit()
{
	SDL_GameControllerClose(controllerHandle);
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int32 getWindowRefreshRate()
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

struct GLTexture
{
	GLuint id;
	int width;
	int height;
};

struct Position
{
	GLfloat x;
	GLfloat y;
};

struct GLColorRGBA
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

//UV coordinates for texture mapping
struct UV
{
	float u;
	float v;
};

struct Vertex
{
	Position pos;
	GLColorRGBA color;
	UV uv;

	void setPosition(float x, float y)
	{
		pos.x = x;
		pos.y = y;
	}

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.b = a;
	}

	void setUV(float u, float v)
	{
		uv.u = u;
		uv.v = v;
	}
};

struct Sprite
{
	vec2 pos{0,0};
	int32 width;
	int32 height;
	//vertex buffer object to hold vertices
	uint32 vboID;

	Sprite()
	{
		//sets the vertex buffer to zero
		vboID = 0;
	}

	~Sprite()
	{
		//delete the buffer
		if (vboID != 0)
		{
			glDeleteBuffers(1, &vboID);
		}
	}

	void create(float32 x, float32 y, float32 w, float32 h)
	{
		pos.x = x;
		pos.y = y;
		width = w;
		height = h;

		if (vboID == 0)
		{
			//generate vertex buffer
			glGenBuffers(1, &vboID);
		}

		//six vertices for a square
		Vertex vertexData[6];

		//first triangle
		vertexData[0].setPosition(x + width, y + height); //top right corner
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//second triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);

		//set colors
		for (int i = 0; i < 6; i++)
		{
			vertexData[i].setColor(255, 0, 255, 255);
		}

		//angle++;
		vertexData[1].setColor(0, 0, 255, 255);
		vertexData[4].setColor(0, 255, 0, 255);

		//bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		//upload the buffer data
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	}

	void draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		//enable the position attribute
		glEnableVertexAttribArray(0);

		//position attribute pointer. tells opengl how our data buffer is structured - point it to the start of the buffer
		//start at 0 using 2 floats
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));

		//color attribute pointer, use normalize = convert numbers to a range from 0 to 1
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));

		//UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
	}
};

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

	const float64 MAX_FRAME_TIME = 1000 / 60;
	int32 refreshRate = getWindowRefreshRate();
	globalTime = SDL_GetTicks(); //LAST_UPDATE_TIME
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
					if (event.key.keysym.sym == SDLK_s)
					{
						SOIL_save_screenshot("screenshot.bmp", SOIL_SAVE_TYPE_BMP, 0, 0, screenWidth, screenHeight);
						LOG("Screenshot saved!\n");
					}

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

		for (int32 i = 1; i <= 3; i++)
		{
			mouseButton[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
		}

		/* the time it takes to draw one frame */
		uint32 timeDelta = SDL_GetTicks() - globalTime;
		if (timeDelta > MAX_FRAME_TIME)
			timeDelta = (uint32)MAX_FRAME_TIME;
		globalTime = SDL_GetTicks();

		updateAndDraw(timeDelta);
		
		uint64 endCycleCount = __rdtsc();
		uint64 endCounter = SDL_GetPerformanceCounter();
		uint64 counterElapsed = endCounter - lastCounter;
		uint64 cyclesElapsed = endCycleCount - lastCycleCount;

		float64 msPerFrame = (((1000.0f * (float64)counterElapsed) / (float64)performanceFrequency));
		float64 fps = (float64)performanceFrequency / (float64)counterElapsed;
		float64 megaCyclesPerFrame = ((float64)cyclesElapsed / (1000.0f * 1000.0f));

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
	quit();
	return 0;
}
