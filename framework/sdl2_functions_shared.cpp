	#pragma comment(lib, "SDL2.lib")
	#pragma comment(lib, "SDL2main.lib")
	#pragma comment(lib, "SDL2_mixer.lib")

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_mixer.h>

#if defined(SDL2)
	#pragma comment(lib, "SDL2_ttf.lib")
	#include <SDL2/SDL_ttf.h>

	SDL_Renderer *renderer = 0;
#elif defined(OPENGL) || defined(OPENGL4)
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
	#pragma comment(lib, "glew32.lib")
	#include <GL/glew.h>
	#include <GL/gl.h>
	#include <gl/glu.h>
	//#include <sdl2/SDL_opengl.h>

	SDL_GLContext glContext;
#endif

	/* Globals */
	SDL_Window *window = 0;
	Mix_Music *music = 0;

enum MouseButtons
	{
		MOUSE_LEFT = 1,
		MOUSE_MIDDLE,
		MOUSE_RIGHT
	};

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
	real32 joyStickX;
	real32 joyStickY;
	real32 joyRightStickX;
	real32 joyRightStickY;

void initInput()
{
	
	/*int32 *numKeys;
	uint8 *keyboard = (uint8*)SDL_GetKeyboardState(numKeys);
	Assert(*numKeys == MAX_KEYS);*/
	for (int32 i = 0; i < MAX_KEYS; i++)
	{
		keys[i] = 0;// keyboard[i];
		prevKeys[i] = 0;
	}

	SDL_GetMouseState(&mouseX, &mouseY);
	prevMouseX = mouseX;
	prevMouseY = mouseY;

	for (int32 i = 0; i <= 3; i++)
	{
		mouseButtons[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
		prevMouseButtons[i] = false;
	}

	controllerHandle = SDL_GameControllerOpen(0);
}

void freeInput()
{
	SDL_GameControllerClose(controllerHandle);
}


void hideCursor(bool hide)
{
	if (hide)
		SDL_ShowCursor(SDL_DISABLE);
	else
		SDL_ShowCursor(SDL_ENABLE);
}



void updateInput()
{
	uint8 *keyboard = (uint8*)SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; i++)
	{
		prevKeys[i] = keys[i];
		keys[i] = keyboard[i];
	}

	//save the mouse coordinates for the previous frame, this is used for mousedrag
	prevMouseX = mouseX;
	prevMouseY = mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);

	for (int i = 0; i <= 3; i++)
	{
		prevMouseButtons[i] = mouseButtons[i];
		mouseButtons[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
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
		joyStickX = processStickValue(joyStickX, stickDeadZone);
		joyStickY = SDL_GameControllerGetAxis(controllerHandle, SDL_CONTROLLER_AXIS_LEFTY);
		joyStickY = processStickValue(joyStickY, stickDeadZone);
		joyRightStickX = SDL_GameControllerGetAxis(controllerHandle, SDL_CONTROLLER_AXIS_RIGHTX);
		joyRightStickX = processStickValue(joyRightStickX, stickDeadZone);
		joyRightStickY = SDL_GameControllerGetAxis(controllerHandle, SDL_CONTROLLER_AXIS_RIGHTY);
		joyRightStickY = processStickValue(joyRightStickY, stickDeadZone);
	}

	if (mouseX != prevMouseX || mouseY != prevMouseY)
		mouseMoved = true;
	else
		mouseMoved = false;

	if (mouseDown(MOUSE_LEFT) && (mouseX != prevMouseX || mouseY != prevMouseY))
		mouseDragged = true;
	else
		mouseDragged = false;

}

//returns the number of milliseconds since start of program
uint32 millis()
{
	return SDL_GetTicks();
}

/* Sound */
void loadPlayMusic(char *filename)
{
	music = Mix_LoadMUS(filename);
	//pointer to Mix_Music to play, loop (-1 = infinite)
	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(30);
}

Mix_Chunk* loadSound(char *filename)
{
	return Mix_LoadWAV(filename);
}

void playSound(Mix_Chunk *snd)
{
	Mix_PlayChannel(-1, snd, 0);
	//Mix_VolumeChunk(snd, 100);
}

/* Sound */
struct sdl_audio_ring_buffer
{
	int Size;
	int WriteCursor;
	int PlayCursor;
	void *Data;
} audioRingBuffer;

enum SoundWaves
{
	SINEWAVE,
	SQUAREWAVE
};
#if 0
struct SoundOutput
{
	int samplesPerSecond;
	real32 toneHz;
	int16 toneVolume;
	uint32 runningSampleIndex;
	int wavePeriod;
	int bytesPerSample;
	int secondaryBufferSize;
	real32 tSine;
	int latencySampleCount;

	SoundWaves soundWaveType = SINEWAVE;

	void play()
	{
		SDL_PauseAudio(0);
	}
	void stop()
	{
		SDL_PauseAudio(1);
	}
	void freq(real32 freq)
	{
		toneHz = freq;
		wavePeriod = soundOutput.samplesPerSecond / soundOutput.toneHz;
	}
} soundOutput;

void audioCallback(void *UserData, Uint8 *AudioData, int Length)
{
	sdl_audio_ring_buffer *RingBuffer = (sdl_audio_ring_buffer *)UserData;

	int Region1Size = Length;
	int Region2Size = 0;
	if (RingBuffer->PlayCursor + Length > RingBuffer->Size)
	{
		Region1Size = RingBuffer->Size - RingBuffer->PlayCursor;
		Region2Size = Length - Region1Size;
	}
	memcpy(AudioData, (uint8*)(RingBuffer->Data) + RingBuffer->PlayCursor, Region1Size);
	memcpy(&AudioData[Region1Size], RingBuffer->Data, Region2Size);
	RingBuffer->PlayCursor = (RingBuffer->PlayCursor + Length) % RingBuffer->Size;
	RingBuffer->WriteCursor = (RingBuffer->PlayCursor + Length) % RingBuffer->Size;
}

void initAudio(int32 samplesPerSecond, int32 bufferSize)
{
	SDL_AudioSpec audioSettings = { 0 };
	/*
	freq: This is the sampling frequency of our sound: the number of samples per second we need to provide.
	format: This is the format of each individual sample. To have signed, 16-bit, little-endian integer samples, we'll use AUDIO_S16LE.
	channels: The number of channels of sound we're providing: 1 for mono, 2 for stereo, more for surround sound setups.
	silence: The recommended value for silence. This is calculated by SDL, we don't need to provide it, and we can work this out from format anyway. (It's usually 0).
	samples: The number of samples we should provide in one go. Higher values will have greater latency, but result in our callback being executed less frequently.
	size: The size in bytes. SDL calculates this for us, we don't need to provide it. It's equal to (the size of a sample in format)*(channels)*(samples).
	callback: A callback function. This function will be called by SDL whenever more audio data is required. It is often called on a different thread to the main game.
	userdata: A pointer of our choice that gets passed to callback. This is a way for us to assosciate data with our sound system without using global variables.
	*/
	audioSettings.freq = samplesPerSecond;
	audioSettings.format = AUDIO_S16LSB;
	audioSettings.channels = 2;
	audioSettings.samples = 512;
	audioSettings.callback = &audioCallback;
	audioSettings.userdata = &audioRingBuffer;

	audioRingBuffer.Size = bufferSize;
	audioRingBuffer.Data = calloc(bufferSize, 1);
	audioRingBuffer.PlayCursor = audioRingBuffer.WriteCursor = 0;

	SDL_OpenAudio(&audioSettings, 0);

	printf("Initialised audio device at frequency %d Hz, %d Channels, buffer size %d\n",
		audioSettings.freq, audioSettings.channels, audioSettings.size);

	if (audioSettings.format != AUDIO_S16LSB)
	{
		printf("Oops! We didn't get AUDIO_S16LSB as our sample format!\n");
		SDL_CloseAudio();
	}

}

void fillSoundBuffer(SoundOutput *soundOutput, int ByteToLock, int BytesToWrite)
{
	//square wave
	int squareWavePeriod = soundOutput->samplesPerSecond / soundOutput->toneHz;
	int halfSquareWavePeriod = squareWavePeriod / 2;

	void *Region1 = (uint8*)audioRingBuffer.Data + ByteToLock;
	int Region1Size = BytesToWrite;
	if (Region1Size + ByteToLock > soundOutput->secondaryBufferSize)
	{
		Region1Size = soundOutput->secondaryBufferSize - ByteToLock;
	}
	void *Region2 = audioRingBuffer.Data;
	int Region2Size = BytesToWrite - Region1Size;
	int Region1SampleCount = Region1Size / soundOutput->bytesPerSample;
	int16 *SampleOut = (int16 *)Region1;
	for (int SampleIndex = 0;
		SampleIndex < Region1SampleCount;
		++SampleIndex)
	{
		real32 SineValue;
		int16 SampleValue;
		if (soundOutput->soundWaveType == SINEWAVE)
		{
			//sine wave
			SineValue = sinf(soundOutput->tSine);
			SampleValue = (int16)(SineValue * soundOutput->toneVolume);
		}
		else
		{
			//square wave
			SampleValue = ((soundOutput->runningSampleIndex / halfSquareWavePeriod) % 2) ? soundOutput->toneVolume : -soundOutput->toneVolume;
		}

		*SampleOut++ = SampleValue;
		*SampleOut++ = SampleValue;

		soundOutput->tSine += 2.0f*PI*1.0f / (real32)soundOutput->wavePeriod;
		++soundOutput->runningSampleIndex;
	}

	int Region2SampleCount = Region2Size / soundOutput->bytesPerSample;
	SampleOut = (int16 *)Region2;
	for (int SampleIndex = 0;
		SampleIndex < Region2SampleCount;
		++SampleIndex)
	{
		real32 SineValue;
		int16 SampleValue;
		if (soundOutput->soundWaveType == SINEWAVE)
		{
			//sine wave
			SineValue = sinf(soundOutput->tSine);
			SampleValue = (int16)(SineValue * soundOutput->toneVolume);
		}
		else
		{
			//square wave
			SampleValue = ((soundOutput->runningSampleIndex / halfSquareWavePeriod) % 2) ? soundOutput->toneVolume : -soundOutput->toneVolume;
		}
		*SampleOut++ = SampleValue;
		*SampleOut++ = SampleValue;

		soundOutput->tSine += 2.0f*PI*1.0f / (real32)soundOutput->wavePeriod;
		++soundOutput->runningSampleIndex;
	}
}
#endif
SDL_Surface* loadSTBImage(char *filename)
{
	int x, y, comp;
	unsigned char *data;
	uint32_t rmask, gmask, bmask, amask;
	SDL_Surface *rv;

	FILE *file = fopen(filename, "rb");
	if (!file)
		return 0;

	data = stbi_load_from_file(file, &x, &y, &comp, 0);
	fclose(file);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	if (comp == 4)
		rv = SDL_CreateRGBSurface(0, x, y, 32, rmask, gmask, bmask, amask);
	else if (comp == 3)
		rv = SDL_CreateRGBSurface(0, x, y, 24, rmask, gmask, bmask, 0);
	else 
	{
		stbi_image_free(data);
		return 0;
	}

	memcpy(rv->pixels, data, comp * x * y);
	stbi_image_free(data);

	return rv;
}

//TODO: make this code non sdl specific
#if defined(OPENGL)
//entity frame of reference
struct Entity3D
{
	vec3 vLocation;	//position in space
	vec3 vUp;		//vector that points upward
	vec3 vForward;	//vector that points forward


	void init()
	{
		vLocation.x = 0.0f;
		vLocation.y = 0.0f;
		vLocation.z = 0.0f;

		vUp.x = 0.0f;
		vUp.y = 1.0f;
		vUp.z = 0.0f;

		vForward.x = 0.0f;
		vForward.y = 0.0f;
		vForward.z = -1.0f;
	}

	//derives a 4x4 transformation matrix from a frame of reference
	void getMatrixFromFrame(Matrix mMatrix)
	{
		vec3 vXAxis;

		//calculate X axis
		vXAxis = vectorCrossProduct(vUp, vForward);

		//x column 
		mMatrix[0] = vXAxis.x;
		mMatrix[1] = vXAxis.y;
		mMatrix[2] = vXAxis.z;
		mMatrix[3] = 0.0f;

		//y column
		mMatrix[4] = vUp.x;
		mMatrix[5] = vUp.y;
		mMatrix[6] = vUp.z;
		mMatrix[7] = 0.0f;

		//z column 
		mMatrix[8] = vForward.x;
		mMatrix[9] = vForward.y;
		mMatrix[10] = vForward.z;
		mMatrix[11] = 0.0f;

		//translation vector
		mMatrix[12] = vLocation.x;
		mMatrix[13] = vLocation.y;
		mMatrix[14] = vLocation.z;
		mMatrix[15] = 1.0f;
	}

	//apply an actors transform given it's frame of reference
	void applyActorTransform()
	{
		Matrix mTransform;
		getMatrixFromFrame(mTransform);
		glMultMatrixf(mTransform);
	}

	//apply a camera transform given a frame of reference, works almost  
	//like gluLookAt
	void applyCameraTransform()
	{
		Matrix mMatrix;
		vec3 vAxisX;
		vec3 zFlipped;

		zFlipped.x = -vForward.x;
		zFlipped.y = -vForward.y;
		zFlipped.z = -vForward.z;

		//derive X vector
		vAxisX = vectorCrossProduct(vUp, zFlipped);

		mMatrix[0] = vAxisX.x;
		mMatrix[4] = vAxisX.y;
		mMatrix[8] = vAxisX.z;
		mMatrix[12] = 0.0f;

		mMatrix[1] = vUp.x;
		mMatrix[5] = vUp.y;
		mMatrix[9] = vUp.z;
		mMatrix[13] = 0.0f;

		mMatrix[2] = zFlipped.x;
		mMatrix[6] = zFlipped.y;
		mMatrix[10] = zFlipped.z;
		mMatrix[14] = 0.0f;

		mMatrix[3] = 0.0f;
		mMatrix[7] = 0.0f;
		mMatrix[11] = 0.0f;
		mMatrix[15] = 1.0f;

		//rotate
		glMultMatrixf(mMatrix);

		//translate backwards
		glTranslatef(-vLocation.x, -vLocation.y, -vLocation.z);
	}

	// move location forward along the forward vector
	void moveForward(real32 fStep)
	{
		vLocation.x += vForward.x * fStep;
		vLocation.y += vForward.y * fStep;
		vLocation.z += vForward.z * fStep;
	}


	//move up the local Y axis
	void moveUp(real32 fStep)
	{
		vLocation.x += vUp.x * fStep;
		vLocation.y += vUp.y * fStep;
		vLocation.z += vUp.z * fStep;
	}

	//move along local X axis
	void moveRight(real32 fStep)
	{
		vec3 vCross;

		vCross = vectorCrossProduct(vUp, vForward);
		vLocation.x += vCross.x * fStep;
		vLocation.y += vCross.y * fStep;
		vLocation.z += vCross.z * fStep;
	}

	//translate world coordinates
	void translateFrameWorld(real32 x, real32 y, real32 z)
	{
		vLocation.x += x;
		vLocation.y += y;
		vLocation.z += z;
	}

	//translate local coordinates
	void translateFrameLocal(real32 x, real32 y, real32 z)
	{
		moveRight(x);
		moveUp(y);
		moveForward(z);
	}

	//rotate around local Y axis
	void rotateLocalY(real32 fAngle)
	{
		Matrix mRotation;
		vec3 vNewForward;

		createRotationMatrix(radians(fAngle), vec3{ 0.0f, 1.0f, 0.0f }, mRotation);
		createRotationMatrix(fAngle, vUp, mRotation);
		rotateVector(vForward, mRotation, &vNewForward);
		vForward = vNewForward;
	}

	//rotate around local X axis
	void rotateLocalX(real32 fAngle)
	{
		Matrix mRotation;
		vec3 vCross;

		vForward = vectorCrossProduct(vCross, vUp);
		createRotationMatrix(fAngle, vCross, mRotation);

		vec3 vNewVect;
		//3x3 matrix multiply for rotation only
		vNewVect.x = mRotation[0] * vForward.x + mRotation[4] * vForward.y + mRotation[8] * vForward.z;
		vNewVect.y = mRotation[1] * vForward.x + mRotation[5] * vForward.y + mRotation[9] * vForward.z;
		vNewVect.z = mRotation[2] * vForward.x + mRotation[6] * vForward.y + mRotation[10] * vForward.z;
		vForward = vNewVect;

		//update pointing up vector
		vNewVect.x = mRotation[0] * vUp.x + mRotation[4] * vUp.y + mRotation[8] * vUp.z;
		vNewVect.y = mRotation[1] * vUp.x + mRotation[5] * vUp.y + mRotation[9] * vUp.z;
		vNewVect.z = mRotation[2] * vUp.x + mRotation[6] * vUp.y + mRotation[10] * vUp.z;
		vUp = vNewVect;
	}

	//rotate a frame around it's local Z axis
	void rotateFrameLocalZ(real32 fAngle)
	{
		Matrix mRotation;

		//only the up vector needs to be rotated
		createRotationMatrix(fAngle, vForward, mRotation);

		vec3 vNewVect;
		vNewVect.x = mRotation[0] * vUp.x + mRotation[4] * vUp.y + mRotation[8] * vUp.z;
		vNewVect.y = mRotation[1] * vUp.x + mRotation[5] * vUp.y + mRotation[9] * vUp.z;
		vNewVect.z = mRotation[2] * vUp.x + mRotation[6] * vUp.y + mRotation[10] * vUp.z;
		vUp = vNewVect;
	}
	float camYaw = 0.0f;       //camera rotation in Y axis in degrees, left/right
	float camPitch = 0.0f;     //camera rotation in X axis in degrees, up/down

	void lockCamera()
	{
		//set campitch between -90 and 90 and set camyaw between 0 and 360 degrees
		if (camPitch > 90)
			camPitch = 90;
		if (camPitch < -90)
			camPitch = -90;
		if (camYaw < 0.0)
			camYaw += 360.0;
		if (camYaw > 360.0)
			camYaw -= 360;
	}


	void update()
	{

		if (keyDown(SDL_SCANCODE_W))
		{
			moveForward(0.1f);
		}
		if (keyDown(SDL_SCANCODE_S))
		{
			moveForward(-0.1f);
		}
		if (keyDown(SDL_SCANCODE_A))
		{
			moveRight(0.1f);
		}
		if (keyDown(SDL_SCANCODE_D))
		{
			moveRight(-0.1f);
		}
		if (keyDown(SDL_SCANCODE_LEFT))
		{
			rotateLocalY(0.1f);
		}
		if (keyDown(SDL_SCANCODE_RIGHT))
		{
			rotateLocalY(-0.1f); //vForward
		}
		camPitch = 0;
		camYaw = 0;
		//bool32 rotFlag = false;
		real32 mouseVel = 0.002f;
		int midX = windowWidth / 2;   //middle of the screen
		int MidY = windowHeight / 2;
		SDL_ShowCursor(SDL_DISABLE);    //we don't show the cursor
		int tmpx, tmpy;
		SDL_GetMouseState(&tmpx, &tmpy); //get the current position of the cursor
		camYaw += mouseVel*(midX - tmpx);   //get the rotation, for example, if the mouse current position is 315, than 5*0.2, this is for Y
		if (midX > tmpx)
		{
			camYaw += mouseVel*(midX - tmpx);   //get the rotation, for example, if the mouse current position is 315, than 5*0.2, this is for Y
			lockCamera();
			rotateLocalY(camYaw);
		}
		else
		{
			camYaw -= mouseVel*(midX - tmpx);   //get the rotation, for example, if the mouse current position is 315, than 5*0.2, this is for Y
			lockCamera();
			rotateLocalY(camYaw);
		}
		//camPitch += mouseVel*(MidY - tmpy); //this is for X
		//lockCamera();
		SDL_WarpMouseInWindow(window, midX, MidY); //center the mousrcursor agin so we get the difference on the next frame
												   //mouse movement
												   //glRotatef(-camPitch, 1.0, 0.0, 0.0);       //rotate the camera (more precisly move everything in the opposit direction)
												   //glRotatef(-camYaw, 0.0, 1.0, 0.0);
												   //if (rotFlag)
												   //rotateLocalY(camYaw);
												   //else
												   //	rotateLocalY(-camYaw);
	}
};

/* Camera */
struct Camera
{
	float camX = 0.0f, camY = 0.0, camZ = 100.0;       //current position of the camera
	float camYaw = 0.0f;       //camera rotation in Y axis in degrees, left/right
	float camPitch = 0.0f;     //camera rotation in X axis in degrees, up/down

	void lockCamera()
	{
		//set campitch between -90 and 90 and set camyaw between 0 and 360 degrees
		if (camPitch > 90)
			camPitch = 90;
		if (camPitch < -20)
			camPitch = -20;
		if (camYaw < 0.0)
			camYaw += 360.0;
		if (camYaw > 360.0)
			camYaw -= 360;
	}

	void moveCamera(float dist, float dir)
	{
		float rad = (camYaw + dir)*(float)M_PI / 180.0f;      //convert the degrees into radians
		camX -= sin(rad)*dist;    //calculate the new coorinate, if you don't understand, draw a right triangle with the datas, you have
		camZ -= cos(rad)*dist;    //and try to calculate the new coorinate with trigonometric functions, that should help
	}

	void moveCameraUp(float dist, float dir)
	{
		//the the same, only this time we calculate the y coorinate
		float rad = (camPitch + dir)*(float)M_PI / 180.0f;
		camY += sin(rad)*dist;
	}

	void updateRotation(float movevel, float mousevel)      //move and mouse sensitivity, and is the mouse inside the window?
	{

		int MidX = windowWidth / 2;   //middle of the screen
		int MidY = windowHeight / 2;
		SDL_ShowCursor(SDL_DISABLE);    //we don't show the cursor
		int tmpx, tmpy;
		SDL_GetMouseState(&tmpx, &tmpy); //get the current position of the cursor
		camYaw += mousevel*(MidX - tmpx);   //get the rotation, for example, if the mouse current position is 315, than 5*0.2, this is for Y
		camPitch += mousevel*(MidY - tmpy); //this is for X
		lockCamera();
		//SDL_WarpMouse(MidX, MidY);       //move back the cursor to the center of the screen
		//SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_WarpMouseInWindow(window, MidX, MidY); //center the mousrcursor agin so we get the difference on the next frame

		const Uint8* keyState = SDL_GetKeyboardState(NULL);     //which key is pressed?
		if (keyState[SDL_SCANCODE_W])
		{
			//detta har att g�r amed n�r man �r ovanifr�n eller undderifr�n ett objekt o g�r fram�t
			//i ett fps spel s� kommenterar man bort if raden och moveCameraUp
			//if (camPitch != 90 && camPitch != -90)       //if we are facing directly up or down, we don't go forward, it will be commented out, when there will be gravity
				moveCamera(movevel, 0.0);        //move forward
			//moveCameraUp(movevel, 0.0);			//move up/down
		}
		else if (keyState[SDL_SCANCODE_S])
		{
			//same, just we use 180 degrees, so we move at the different direction (move back)
			//if (camPitch != 90 && camPitch != -90)
				moveCamera(movevel, 180.0);
			//moveCameraUp(movevel, 180.0);
		}
		if (keyState[SDL_SCANCODE_A])       //move left
			moveCamera(movevel, 90.0);
		else if (keyState[SDL_SCANCODE_D])  //move right
			moveCamera(movevel, 270);

		//mouse movement
		glRotatef(-camPitch, 1.0, 0.0, 0.0);       //rotate the camera (more precisly move everything in the opposit direction)
		glRotatef(-camYaw, 0.0, 1.0, 0.0);
	}

	//key movement
	void updateTranslation()
	{
		glTranslatef(-camX, -camY, -camZ);        //move the camera
	}
} cam;
#endif