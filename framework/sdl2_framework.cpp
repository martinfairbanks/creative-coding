void screen(int32 width, int32 height, bool32 screen, char *title)
{
	//retrieves the frequency of the performance counter in counts per seonds
	state.performanceFrequency = SDL_GetPerformanceFrequency();

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO);

	int32 tempWidth = windowWidth = width;
	int32 tempHeight = windowHeight = height;

	center = { real32(width / 2), real32(height / 2) };
	
	#if defined(OPENGL) || defined(OPENGL4)
		uint32 flags = SDL_WINDOW_OPENGL;// | SDL_WINDOW_RESIZABLE;
	#else
		uint32 flags = SDL_WINDOW_SHOWN;//SDL_WINDOW_RESIZABLE
	#endif

	state.fullscreen = screen;

	//set to zero to scale the window to desired resolution without changing the desktop resolution
	if (state.fullscreen)
	{
		#if defined(OPENGL)
			flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
		#else
			flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
		#endif
		tempWidth = 0;
		tempHeight = 0;
	}

#if 0
	int32 tempwindowWidth = GetSystemMetrics(SM_CXSCREEN);
	int32 tempwindowHeight = GetSystemMetrics(SM_CYSCREEN);

	int32 windowPosX = tempwindowWidth / 2 - tempWidth / 2;
	int32 windowPosY = tempwindowHeight / 2 - tempHeight / 2;

	//prevent window title bar from being shown outside of screen
	if (windowPosY < 40)
		windowPosY = 40;
#endif		
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, tempWidth, tempHeight, flags);

#if defined(OPENGL)
	glContext = SDL_GL_CreateContext(window);
	//use doublebuffer
//	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (state.vSync)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);
	
	//blending
	glEnable(GL_BLEND);
	//color = (a*source)+(b*dest)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	set2dProjection();
#elif defined(OPENGL4)
	glContext = SDL_GL_CreateContext(window);

	if (state.vSync)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);
	
	//initialize GLEW
	glewInit();

	//define the viewport dimensions
	glViewport(0, 0, windowWidth, windowHeight);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	
	if (!glewIsSupported("GL_VERSION_4_5"))
	{
		printf("OpenGL 4.5 is not available!\n");
	}
#if 0
	/* build and compile shader program */
	
	/*colorShader.loadShader("data/shaders/colorshader.vert", "data/shaders/colorshader.frag");
	colorShader.linkShaders();

	textureShader.loadShader("data/shaders/textureshader.vert", "data/shaders/textureshader.frag");
	textureShader.linkShaders();*/
	
	colorShader.loadShader(vertexShaderSource, fragmentShaderSource);
	colorShader.linkShaders();


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
	
	/*
	real32 vertices[] =
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
	/*int textureWidth, textureHeight;
	unsigned char* image = stb_load_image("data/container.jpg", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.*/
#endif
#else
	if (state.vSync)
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	else
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (state.fullscreen)
	{
		//scale window to desired resolution
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);
	}

	backbufferTexture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		windowWidth, windowHeight);

	TTF_Init();
	int32 fontSize = 6;
	for (int32 i = 0; i < numFonts; i++)
	{
		font[i] = TTF_OpenFont("data/fonts/verdana.ttf", fontSize);
		//printf("The font max height is: %d\n", TTF_FontHeight(font[i]));
		fontSize += 6;
	}

	SDL_SetRenderDrawColor(renderer, 0x65, 0x9C, 0xEF, 255);
	SDL_RenderClear(renderer);
#endif

	pixels = (uint32 *)malloc(windowWidth * windowHeight * 4);
	//pixels = new unsigned int[windowWidth * windowHeight];	
	//pixels = (uint32 *)VirtualAlloc(0, windowWidth * windowHeight * bytesPerPixel, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	SDL_Surface *surface = loadSTBImage("data/pix/icon.png");
	SDL_SetWindowIcon(window, surface);
	SDL_FreeSurface(surface);

	//initialize audio. sound frequency, sound format, channels( 2=stereo), sample size
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		printLogError("Failed to initialize SDL_Mixer!");


	initInput();
	randomSeed(SDL_GetTicks());
	//std::default_random_engine generator(SDL_GetTicks());
}

void quit()
{
	shutdown();
	#if defined(OPENGL) || defined(OPENGL4)
		SDL_GL_DeleteContext(glContext);
	#else
		for (int32 i = 0; i < numFonts; i++)
		{
			TTF_CloseFont(font[i]);
		}
		TTF_Quit();
		SDL_DestroyTexture(backbufferTexture);
		SDL_DestroyRenderer(renderer);
	#endif

	freeInput();
	free(pixels);
	Mix_FreeMusic(music);
	Mix_Quit();
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

real32 getSecondsElapsed(uint64 OldCounter, uint64 CurrentCounter)
{
	return ((real32)(CurrentCounter - OldCounter) / (real32)state.performanceFrequency);
}

int main(int argc, char** argv)
{
	//Check for memory leak if debug build
#if defined(_DEBUG) || defined(FRAMEWORK_INTERNAL)     
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif	

	//TODO: fix assertion macro with message box
	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "", "", NULL);
	setup();
	real32 targetSecondsPerFrame;
	int32 refreshRate = getWindowRefreshRate();
	printf("Refresh rate is %d Hz\n", refreshRate);

	if (framesPerSecond)
		targetSecondsPerFrame = 1.0f / (real32)framesPerSecond;
	else
		targetSecondsPerFrame = 1.0f / (real32)refreshRate;

	//this is the time it takes to draw a frame
	//this allows us to run the game in any framerate we want
	deltaTime = targetSecondsPerFrame;

	//get the current value of the high resolution counter
	uint64 lastCounter = SDL_GetPerformanceCounter();
	uint64 lastCycleCount = __rdtsc();
	SDL_Event event;
	state.running = true;

	 //Enable text input
	SDL_StartTextInput();
  
	while (state.running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				#if defined(OPENGL)
				//get new dimensions and repaint on window size change  
				case SDL_WINDOWEVENT:
				{
					if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						windowWidth = event.window.data1;
						windowHeight = event.window.data2;
						if (projection3DFlag)
							set3dProjection();
						else
							set2dProjection();
					}
				} break;
				#endif
				case SDL_MOUSEMOTION:
				{
					//NOTE: this works in windowed and fullscreen mode
					//mouseX = event.button.x;
					//mouseY = event.button.y;
				} break;
				case SDL_KEYDOWN:
				{
					//register keypress to the GUI
					state.keyentered = event.key.keysym.sym;
					//state.keymod = event.key.keysym.mod;
					//state.keychar = event.text.text[0];
					switch (event.key.keysym.sym)
					{
						
						case SDLK_RETURN:
						{
							//grab mouse to prevent unexpected movement when the dimensions of the window changes
							//TODO: check, hides mousecursor?
							//SDL_SetRelativeMouseMode(SDL_TRUE);
							#if defined(OPENGL) || defined(OPENGL4)
								if (state.fullscreen)
								{
									SDL_SetWindowFullscreen(window, SDL_FALSE);
									state.fullscreen = false;
								}
								else
								{
									//TODO:fix this, is there a windowSetScale?
									SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
									state.fullscreen = true;
								}
							#else
								if (state.fullscreen)
								{
									SDL_RenderSetScale(renderer, 1.0f, 1.0f);
									SDL_SetWindowFullscreen(window, SDL_FALSE);
									SDL_ShowCursor(1);
									state.fullscreen = false;
								}
								else
								{
									//scale window to desired resolution
									//SDL_SetWindowFullscreen(window, SDL_TRUE);
									SDL_RenderSetScale(renderer, 2.0f, 2.0f);
									SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
									SDL_ShowCursor(0);
									state.fullscreen = true;
								}
							#endif
						}
					} break;
				} break;

				case SDL_KEYUP:
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
						state.running = false;
					
					//save software rendered pixels
					if (event.key.keysym.sym == SDLK_z)
					{
						#if defined(OPENGL)
							uint32 *buf = (uint32 *)malloc(windowWidth * windowHeight * 4);				
							uint32 *tempBuf = (uint32 *)malloc(windowWidth * windowHeight * 4);				
							glPixelStorei(GL_PACK_ALIGNMENT, 1);

							//returns the image flipped vertically
							glReadPixels(0, 0, windowWidth, windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, &buf[0]);
							//flip image vertically
							for (int32 y = 0; y < windowHeight; y++)
							{
								for (int32 x = 0; x < windowWidth; x++)
								{
									tempBuf[y*windowWidth + x] = buf[(windowHeight - y - 1)*windowWidth + x];
								}
							}
							stbi_write_bmp("screenshot.bmp", windowWidth, windowHeight, 4, &tempBuf[0]);
							free(buf);
							free(tempBuf);
						#elif defined(SDL2)
							SDL_Surface* tempSurface = NULL;
							SDL_Surface* windowSurface = NULL;
							windowSurface = SDL_GetWindowSurface(window);

							SDL_RenderReadPixels(renderer, &windowSurface->clip_rect, windowSurface->format->format,
								pixels, windowSurface->w * windowSurface->format->BytesPerPixel);

							tempSurface = SDL_CreateRGBSurfaceFrom(
								pixels, windowSurface->w, windowSurface->h, windowSurface->format->BitsPerPixel,
								windowSurface->w * windowSurface->format->BytesPerPixel, windowSurface->format->Rmask,
								windowSurface->format->Gmask, windowSurface->format->Bmask, windowSurface->format->Amask);

							SDL_SaveBMP(tempSurface, "screenshot.bmp");
							SDL_FreeSurface(tempSurface);
							tempSurface = NULL;

							SDL_FreeSurface(windowSurface);
							windowSurface = NULL;
						#endif
					}

					//TODO: stb saves in RGBA format, two options to fix this
					//1. change the backbufferTexture to SDL_PIXELFORMAT_RGBA8888 and change all pixel functions to map that format
					//2. loop through the pixels before save and swap the byte order
					
					//Captures the OpenGL window(RGB) and saves it to disk.
					//SOIL_save_screenshot("test.bmp", SOIL_SAVE_TYPE_BMP, 0, 0, windowWidth, windowHeight);
					//SOIL_save_image("test.bmp", SOIL_SAVE_TYPE_BMP, windowWidth, windowHeight, 4, (const unsigned char*)pixels);
					//stbi_write_jpg("test.jpg", windowWidth, windowHeight, 4, (void *)backbufferTexture, 50);
					//stbi_write_bmp("test.bmp", windowWidth, windowHeight, 4, pixels);
					//stbi_write_png("test.png", windowWidth, windowHeight, 4, pixels, windowWidth*4);
					//LOG("Screenshot saved!\n");
					
				} break;

				case SDL_TEXTINPUT:
				{
					//Not copy or pasting
					/*if (!((event.text.text[0] == 'c' || event.text.text[0] == 'C') && (event.text.text[0] == 'v' || event.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
					{
						if (event.text.text[0] == '\n')
							__debugbreak;
						//Append character
						inputText += event.text.text;
						renderText = true;
					}*/

					//register keypress to the GUI
					state.keyentered = event.key.keysym.sym;
					state.keymod = event.key.keysym.mod;
					state.keychar = event.text.text[0];
					// if key is ASCII, accept it as character input
					//if ((event.key.keysym.unicode & 0xFF80) == 0)
					//	state.keychar = event.key.keysym.unicode & 0x7f; 
						

				} break;

				case SDL_QUIT:
				{
					state.running = false;
				} break;
			}
		}
#ifdef SDL2
		stringBatchCount = 0;
		SDL_RenderClear(renderer);
#endif
		updateInput();
		#ifdef OPENGL
			glLoadIdentity();
		#endif
		updateAndDraw();

		//update screen
		#if defined(OPENGL) || defined(OPENGL4)
			/*if (doubleBufferDisabledFlag)
				glFlush();
			else*/
				//update screen
				SDL_GL_SwapWindow(window);
		#else
			if (state.softwareRenderer)
				uploadPixels();
			for (int32 i = 0; i < stringBatchCount; i++)
				print(stringBatch[i]);

			stringBatchCount = 0;
			SDL_RenderPresent(renderer);
		#endif
#if 0
		// Sound output test
		SDL_LockAudio();
		int ByteToLock = (soundOutput.runningSampleIndex*soundOutput.bytesPerSample) % soundOutput.secondaryBufferSize;
		int TargetCursor = ((audioRingBuffer.PlayCursor +
			(soundOutput.latencySampleCount*soundOutput.bytesPerSample)) %
			soundOutput.secondaryBufferSize);
		int BytesToWrite;
		if (ByteToLock > TargetCursor)
		{
			BytesToWrite = (soundOutput.secondaryBufferSize - ByteToLock);
			BytesToWrite += TargetCursor;
		}
		else
		{
			BytesToWrite = TargetCursor - ByteToLock;
		}

		SDL_UnlockAudio();
		fillSoundBuffer(&soundOutput, ByteToLock, BytesToWrite);
#endif
		if (getSecondsElapsed(lastCounter, SDL_GetPerformanceCounter()) < targetSecondsPerFrame)
		{
			int32 timeToSleep = (int32)(((targetSecondsPerFrame - getSecondsElapsed(lastCounter, SDL_GetPerformanceCounter())) * 1000) - 1);
			if (timeToSleep > 0)
			{
				SDL_Delay(timeToSleep);
			}
			//Assert(getSecondsElapsed(lastCounter, SDL_GetPerformanceCounter()) < targetSecondsPerFrame)
			while (getSecondsElapsed(lastCounter, SDL_GetPerformanceCounter()) < targetSecondsPerFrame)
			{
				//wait
			}
		}

		//get the current value of the high resolution counter
		uint64 endCounter = SDL_GetPerformanceCounter();

#if defined(SDL2) && ((_DEBUG) || defined(FRAMEWORK_INTERNAL)) 
		print(debugMessage, 10, 10, 12);
		SDL_SetWindowTitle(window, debugMessage);
#endif
		uint64 endCycleCount = __rdtsc();
		uint64 counterElapsed = endCounter - lastCounter;
		uint64 cyclesElapsed = endCycleCount - lastCycleCount;
		real64 msPerFrame = (((1000.0f * (real64)counterElapsed) / (real64)state.performanceFrequency));
		real64 fps = (real64)state.performanceFrequency / (real64)counterElapsed;
		real64 megaCyclesPerFrame = ((real64)cyclesElapsed / (1000.0f * 1000.0f));

#if defined(_DEBUG) || defined(FRAMEWORK_INTERNAL)       
		sprintf_s(debugMessage, "%.03fms, %.03fFPS, %.03fMEGAc/f, RefreshRate: %d, FrameCount: %llu, perFreq: %llu, endCounter: %llu, elapsed: %llu\0", msPerFrame, fps, megaCyclesPerFrame, refreshRate, frameCount, state.performanceFrequency, endCounter, counterElapsed);
#endif
	
		lastCycleCount = endCycleCount;
		lastCounter = endCounter;
		//one frame drawn
		frameCount++;
	}

	quit();
	return 0;
}

