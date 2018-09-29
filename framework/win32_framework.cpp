	/* TODO:	setColor_
				fix fullscreen mode
				fix window paint, window turns black in some cases when you move it
	*/

	/* Linking */
	#pragma comment(lib, "user32.lib")
	#pragma comment(lib, "gdi32.lib")
	#pragma comment(lib, "winmm.lib")		//timeBeginPeriod
	#pragma comment(lib, "Xinput9_1_0.lib") //Xinput.lib

	/* Includes */
	#define WIN32_LEAN_AND_MEAN				//#define WIN32_LEANER_AND_MEANER //#define VC_EXTRALEAN
	#define WIN32_LEANER_AND_MEANER
	#include <Windows.h>
	#include <Xinput.h>
	#include <mmsystem.h>					//beacuse of LEAND_AND_MEAN this is not included in windows.h
	#if defined(OPENGL_WIN32)
		#pragma comment(lib, "opengl32.lib")
		#pragma comment(lib, "glu32.lib")
		#pragma comment(lib, "glew32.lib")
		#include <GL/gl.h>
		#include <GL/glu.h>
		#include "opengl_functions.cpp"
		HGLRC hRC = NULL;				//OpenGL rendering context
	#endif

	#if defined(OPENGL4_WIN32)
		#pragma comment(lib, "opengl32.lib")
		#pragma comment(lib, "glew32.lib")
		#pragma comment(lib, "glfw3.lib")
		#include <GL/glew.h> // include GLEW and new version of GL on Windows
		#include <GLFW/glfw3.h> // GLFW helper library
		#include <GL/gl.h>
		#include "gl/glext.h"
		//#include "opengl4_functions.cpp"
		HGLRC hRC = NULL;				//OpenGL rendering context
	#endif

	/* Globals */
	HWND		window = 0;					//handle to the window
	HINSTANCE	instance;					//handle to this application
	HDC			deviceContext = 0;			//GDI Device Context
	TEXTMETRIC	tm;							//system font
	WINDOWPLACEMENT globalWindowPosition = {sizeof(globalWindowPosition)};

	struct WindowDimension
	{
		int32 width;
		int32 height;
	};

	WindowDimension winDimension;

	struct screenBuffer
	{
		BITMAPINFO info;					//backbuffer bitmap info structer
		void *memory;						//pixel memory
		int pitch;
		int32 width;
		int32 height;
	};

	static screenBuffer backBuffer;

	enum MouseButtons
	{
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT
	};

	int32 joyUp;
	int32 joyDown;
	int32 joyLeft;
	int32 joyRight;
	int32 joyStart;
	int32 joyBack;
	int32 joyLeftShoulder;
	int32 joyRightShoulder;
	int32 joyAButton;
	int32 joyBButton;
	int32 joyXButton;
	int32 joyYButton;
	real32 joyStickX;
	real32 joyStickY;

/* Win32 drawing functions */
#if defined(WIN_32)
inline void clear_(Color col)
{
	HBRUSH brush = CreateSolidBrush(RGB(col.r, col.g, col.b));
	SelectObject(deviceContext, brush);
	Rectangle(deviceContext, 0, 0, windowWidth, windowHeight);
}

inline void pixel_(int32 x, int32 y)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;
	SetPixel(deviceContext, x, y, RGB(strokeColor.r, strokeColor.g, strokeColor.b));
}

inline void pixel_(int32 x, int32 y, uint32 col)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;
	uint8 r = (uint8)(col >> 16);
	uint8 g = (uint8)(col >> 8);
	uint8 b = (uint8)col;
	SetPixel(deviceContext, x, y, RGB(r, g, b));
}

inline void pixel_(int32 x, int32 y, uint8 r, uint8 g, uint8 b)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;
	SetPixel(deviceContext, x, y, RGB(r, g, b));
}

inline void pixel_(int32 x, int32 y, Color col)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;
	SetPixel(deviceContext, x, y, RGB(col.r, col.g, col.b));
}

inline void line_(int32 x0, int32 y0, int32 x1, int32 y1)
{
	//create a colored pen
	HPEN hpen = CreatePen(PS_SOLID, 1, RGB(strokeColor.r, strokeColor.g, strokeColor.b));

	//select pen and save the old one
	HPEN oldPen = (HPEN)SelectObject(deviceContext, hpen);

	//move postion
	MoveToEx(deviceContext, x0, y0, NULL);
	//draw a line
	LineTo(deviceContext, x1, y1);

	//restore old pen
	SelectObject(deviceContext, oldPen);
	//delete pen
	DeleteObject(hpen);
}

inline void rect_(int32 x, int32 y, int32 width, int32 height)
{
	HBRUSH brush = CreateSolidBrush(RGB(strokeColor.r, strokeColor.g, strokeColor.b)); 
	SelectObject(deviceContext, brush); 
	Rectangle(deviceContext, x, y, x + width, y + height);
}

//TODO: Check: SelectObject.
inline void circle_(int32 x, int32 y, int32 radius)
{
	HPEN oldpen = CreatePen(PS_SOLID, 1, RGB(strokeColor.r, strokeColor.g, strokeColor.b));
	
	if (state.fillFlag)
	{
		//HBRUSH brush = CreateSolidBrush(RGB(color.r, color.g, color.b));
		SelectObject(deviceContext, CreateSolidBrush(RGB(strokeColor.r, strokeColor.g, strokeColor.b)));
		Ellipse(deviceContext, x, y, x + radius, y + radius);
		//DeleteObject(brush);
	}
	else
	{
		HBRUSH oldbrush = (HBRUSH)SelectObject(deviceContext, GetStockObject(NULL_BRUSH));
		Ellipse(deviceContext, x, y, x + radius, y + radius);
		SelectObject(deviceContext, oldbrush);
		//DeleteObject(oldbrush);
	}
	SelectObject(deviceContext, oldpen);
	DeleteObject(oldpen);
}

inline void uploadPixels()
{
	#if 0
	//rect to rect copy,copy from one buffer to another and stretches it if needed to fit the destination
	StretchDIBits(deviceContext,
		0, 0, windowWidth, windowHeight,	//destination rect - window 
		0, 0, windowWidth, windowHeight,	//source rect - backbuffer
		backBuffer.memory,					//pointer to image bits stored in an array of bytes
		&backBuffer.info,
		DIB_RGB_COLORS,						//color format 
		SRCCOPY);							//raster-operation 

	#endif
	//if we are in fullscreen
	if((winDimension.width > backBuffer.width) &&
	   (winDimension.height > backBuffer.height))
	{
		/*StretchDIBits(deviceContext,
					  0, 0, 3*buffer->width, 3*buffer->height,
					  0, 0, buffer->width, buffer->height,
					  buffer->memory,
					  &buffer->info,
					  DIB_RGB_COLORS, SRCCOPY);*/
		StretchDIBits(deviceContext,
			0, 0, winDimension.width, winDimension.height,
			0, 0, backBuffer.width, backBuffer.height,
			backBuffer.memory,
			&backBuffer.info,
			DIB_RGB_COLORS, SRCCOPY);
	}
	else
	{
		
		StretchDIBits(deviceContext,
			//x, y, width, height,		//destination rect - window
			//x, y, width, height,		//source rect - backbuffer
			0, 0, backBuffer.width, backBuffer.height, /*windowWidth, windowHeight*/
			0, 0, backBuffer.width, backBuffer.height,
			backBuffer.memory,				//pointer to image bits stored in an array of bytes
			&backBuffer.info,
			DIB_RGB_COLORS,				//RGB color format 
			SRCCOPY);					//raster-operation, copy source rect to dest rect
	}
}
#endif


#if defined(OPENGL_WIN32)
uint32	base;				//font base display list
void buildFont(char *fontName, int fontSize)
{
	HFONT font;

	base = glGenLists(96);								// Storage For 96 Characters
	/*
	You'll notice it's a negative number. By putting a minus, we're telling windows to find us a font based on the CHARACTER height. If we use a positive number we match the font based on the CELL height.
	*/
	font = CreateFont(-fontSize,				// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_BOLD,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision, True Type
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,	// Family And Pitch
		fontName);						// Font Name

#if 0
	// a font such as Webdings or Wingdings, you need to use SYMBOL_CHARSET instead of ANSI_CHARSET.
	if (!stricmp(fontName, "symbol"))
	{
		hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			SYMBOL_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
			fontName);
	}
	else
	{
		hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
			fontName);
	}

	if (!hFont)
		return 0;
#endif
	SelectObject(deviceContext, font);           // Selects The Font We Want
	wglUseFontBitmaps(deviceContext, 32, 96, base);				// Builds 96 Characters Starting At Character 32
}

void freeFont()									// Delete The Font List
{
	if (base != 0)
		glDeleteLists(base, 96);							// Delete All 96 Characters
}

void print(const char *fmt, ...)						// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	vsprintf(text, fmt, ap);							// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}

void print(int xPos, int yPos, const char *str, ...)
{
	if ((base == 0) || (!str))
		return;
	
	va_list		ap;										// Pointer To List Of Arguments
	char		text[256];								

	va_start(ap, str);									// Parses The String For Variables
	vsprintf(text, str, ap);							// And Converts Symbols To Actual Numbers
	va_end(ap);

	glRasterPos2i(xPos, yPos);

	//We then push the GL_LIST_BIT, this prevents glListBase from affecting any other display lists we may be using in our program.
	glPushAttrib(GL_LIST_BIT);
	//starting at character 32
	glListBase(base - 32);
	/*
	 First it tells OpenGL we're going to be displaying lists to the screen. strlen(text) finds out how many letters we're going to send to the screen. Next it needs to know what the largest list number were sending to it is going to be. We're not sending any more than 255 characters. The lists parameter is treated as an array of unsigned bytes, each in the range 0 through 255. Finally we tell it what to display by passing text (pointer to our string).
	 */
	glCallLists((int)strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}
#endif

#if 0
inline void print(char *message, int x, int y)
{
	SetBkMode(deviceContext, OPAQUE); //TRANSPARENT
	SetTextColor(deviceContext, RGB(color.r, color.g, color.b));
	SetBkColor(deviceContext, RGB(0, 0, 0));
	TextOut(deviceContext, x, y, message, strlen(message));	
}
#endif

int64 globalPerfCountFrequency;			//the frequency of the performance counter in counts per seonds
inline real32 win32GetSecondsElapsed(LARGE_INTEGER start, LARGE_INTEGER end)
{
	real32 result = ((real32)(end.QuadPart - start.QuadPart) / (real32)state.performanceFrequency);
	return result;
}

//returns the number of milliseconds since system start
uint32 millis()
{
	return GetTickCount();
}

void initUpdate()
{
	for (int32 i = 0; i<MAX_KEYS; i++)
	{
		keys[i] = false;
		prevKeys[i] = false;
	}

	for (int32 i = 0; i <= 3; i++)
	{
		mouseButtons[i] = false;
		prevMouseButtons[i] = false;
	}
}

void hideCursor(bool hide)
{
	if (hide)
		ShowCursor(false);		
	else
		ShowCursor(true);		
}

void updateInput()
{
	BYTE keyboard[MAX_KEYS];
	GetKeyboardState(keyboard);

	for (int i = 0; i < MAX_KEYS; i++)
	{
		prevKeys[i] = keys[i];
		keys[i] = keyboard[i] >> 7;
	}

	//save the mouse coordinates for the previous frame, this is used for mousedrag
	prevMouseX = mouseX;
	prevMouseY = mouseY;

	//get the mouse position
	POINT mouseP;
	GetCursorPos(&mouseP);
	//map the mouse pointer into the drawable window
	ScreenToClient(window, &mouseP);

	mouseX = mouseP.x;
	mouseY = mouseP.y;
	
	for (int i = 0; i <= 3; i++)
	{
		prevMouseButtons[i] = mouseButtons[i];
	}

	//the highorder bit is set to 1 if the button is down
	//GetKeyState returns a short
	mouseButtons[0] = GetKeyState(VK_LBUTTON) & (1 << 15); //SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
	mouseButtons[1] = GetKeyState(VK_MBUTTON) & (1 << 15);
	mouseButtons[2] = GetKeyState(VK_RBUTTON) & (1 << 15);

	//loop through the game controllers and get it's state
	for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; ++controllerIndex)
	{
		XINPUT_STATE controllerState;
		if (XInputGetState(controllerIndex, &controllerState) == ERROR_SUCCESS)
		{
			//controller is plugged in
			XINPUT_GAMEPAD *pad = &controllerState.Gamepad;
			joyUp = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
			joyDown = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
			joyLeft = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
			joyRight = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
			joyStart = (pad->wButtons & XINPUT_GAMEPAD_START);
			joyBack = (pad->wButtons & XINPUT_GAMEPAD_BACK);
			joyLeftShoulder = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
			joyRightShoulder = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
			joyAButton = (pad->wButtons & XINPUT_GAMEPAD_A);
			joyBButton = (pad->wButtons & XINPUT_GAMEPAD_B);
			joyXButton = (pad->wButtons & XINPUT_GAMEPAD_X);
			joyYButton = (pad->wButtons & XINPUT_GAMEPAD_Y);

			joyStickX = pad->sThumbLX;
			joyStickX = processStickValue(joyStickX, stickDeadZone);
			joyStickY = pad->sThumbLY;
			joyStickY = processStickValue(joyStickX, stickDeadZone);
		}
		else
		{
			//controller is not available
		}
	}

	// Rumble
	/*XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = 60000;
	vibration.wRightMotorSpeed = 60000;
	XInputSetState(0, &vibration);*/

	if (mouseX != prevMouseX || mouseY != prevMouseY)
		mouseMoved = true;
	else
		mouseMoved = false;

	if (mouseDown(MOUSE_LEFT) && (mouseX != prevMouseX || mouseY != prevMouseY))
		mouseDragged = true;
	else
		mouseDragged = false;
}

void toggleFullscreen()
{
	//NOTE: This code is from this article by Raymond Chen
	//http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx
	
	DWORD style = GetWindowLong(window, GWL_STYLE);
	if(style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO monitorInfo = {sizeof(monitorInfo)};
		if(GetWindowPlacement(window, &globalWindowPosition) &&
		   GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &monitorInfo))
		{
			SetWindowLong(window, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(window, HWND_TOP,
						 monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,
						 monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
						 monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
						 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else
	{
		SetWindowLong(window, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(window, &globalWindowPosition);
		SetWindowPos(window, 0, 0, 0, 0, 0,
					 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
					 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

WindowDimension getWindowDimension()
{
	WindowDimension result;

	RECT clientRect;
	GetClientRect(window, &clientRect);	//gets the draw area of the window
	result.width = clientRect.right;// - clientRect.left; //is always 0
	result.height = clientRect.bottom;// - clientRect.top;
	return result;
}

//Message handler callback function.
//Called by DispatchMessage() when the application receives a message.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//0 = we handled the message
	LRESULT result = 0;

	switch (message)
	{
		case WM_SIZE:
		{
			#if defined(OPENGL_WIN32)
				if (projection3DFlag)
					set3dProjection(LOWORD(lParam), HIWORD(lParam));
				else
					set2dProjection(LOWORD(lParam), HIWORD(lParam));
			#endif
		} break;

		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
				PostQuitMessage(0);
			
			bool32 altKeyWasDown = lParam & (1 << 29);
			if((wParam == VK_RETURN) && altKeyWasDown)
			{
				toggleFullscreen();
			}
		} break;

		case WM_KEYUP:
		{
			#if defined(OPENGL_WIN32)
				if (wParam == 'S')
				{
					uint32 *buf = (uint32 *)malloc(windowWidth * windowHeight * 4);				
					uint32 *tempBuf = (uint32 *)malloc(windowWidth * windowHeight * 4);				
					
					glPixelStorei(GL_PACK_ALIGNMENT, 1);

					glReadPixels(0, 0, windowWidth, windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, &buf[0]);
					for (int32 y = 0; y < windowHeight; y++)
					{
						for (int32 x = 0; x < windowWidth; x++)
						{
							tempBuf[y*windowWidth+x] = buf[(windowHeight-y-1)*windowWidth +x];
						}
					}
					stbi_write_bmp("test.bmp", windowWidth, windowHeight, 4, &tempBuf[0]);
					free(buf);
					free(tempBuf);
				}
			#endif
			
		} break;

		case WM_DESTROY:	
		{
		} break;

		case WM_CLOSE:		
		{
			PostQuitMessage(0);
		} break;

		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			deviceContext = BeginPaint(hWnd, &paint);
			EndPaint(hWnd, &paint);
		}
		case WM_MOUSEWHEEL:
		{
			//mouseWheel += GET_WHEEL_DELTA_WPARAM(WParam) > 0 ? +1.0f : -1.0f;
			//return true;
		} break;

		case WM_MOUSELEAVE:
		{
			mouseButtons[MOUSE_LEFT] = false;
			mouseButtons[MOUSE_RIGHT] = false;
			mouseButtons[MOUSE_MIDDLE] = false;
		} break;

		case WM_LBUTTONDBLCLK: break;

		default:
		{
			result = DefWindowProc(hWnd, message, wParam, lParam); //let windows take care of any unhandled messages
		} break;
	}
	return result;
}

void screen(int width, int height, bool32 screen, char *title)
{
	LARGE_INTEGER perfCountFrequencyResult;
	//retrieves the frequency of the performance counter in counts per seonds
	QueryPerformanceFrequency(&perfCountFrequencyResult);
	state.performanceFrequency = perfCountFrequencyResult.QuadPart;

	windowWidth = width;
	windowHeight = height;
	center = { real32(width / 2), real32(height / 2) };
	state.fullscreen = screen;
	

	#if defined(OPENGL_WIN32)
		title = "OpenGL-Framework";
	#else
		title = "Win32-FrameWork";	
	#endif

	//create, initialize and register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0, 0,
		instance, NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL, "creative-framework", NULL };
	RegisterClassExA(&wc);

#if defined(OPENGL_WIN32)
	DWORD dwStyle;						//window style flag
	GLuint pixelFormat;					//OpenGL pixelformat

		if (state.fullscreen)
	{											
		//device mode
		DEVMODE dmScreenSettings;	
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = windowWidth;
		dmScreenSettings.dmPelsHeight = windowHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//try to change to fullscreen
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {

			//failed to change display mode
			MessageBox(NULL, "ChangeDisplaySettings failed! Switch to windowed mode.", NULL, MB_OK);
			state.fullscreen = false;
		}
	}

	if (state.fullscreen)
	{											
		dwStyle = WS_POPUP;										
		ShowCursor(FALSE);										
	}
	else
	{
		dwStyle = WS_OVERLAPPEDWINDOW;							
	}

	window = CreateWindowExA(0, wc.lpszClassName, title, dwStyle /* | WS_VISIBLE*/,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, instance, 0);

	//define the pixelformat for OpenGL, double buffering and RGBA
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);				
	pfd.nVersion = 1;
	pfd.dwFlags =	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;		
	pfd.iPixelType = PFD_TYPE_RGBA;			
	pfd.cColorBits = 32;						//bits per pixel
	pfd.cDepthBits = 8;							//depth of the z-buffer
	pfd.dwLayerMask = PFD_MAIN_PLANE;			//sets the pfd to be the main drawing plane

	deviceContext = GetDC(window);	

	//get the best matching pixel format
	pixelFormat = ChoosePixelFormat(deviceContext, &pfd);
	
	//try to set the pixel format on the window
	SetPixelFormat(deviceContext, pixelFormat, &pfd);

	//create a opengl rendering context
	hRC = wglCreateContext(deviceContext);

	//makes the specified OpenGL rendering context the calling thread's current rendering context
	wglMakeCurrent(deviceContext, hRC);
	
	ShowWindow(window, SW_SHOW);

	//blending
	glEnable(GL_BLEND);
	//color = (a*source)+(b*dest)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//set2dProjection();
	
	buildFont("Verdana", 32);
#elif defined(OPENGL4_WIN32)
	DEVMODE dmScreenSettings = {};
	dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
	dmScreenSettings.dmPelsWidth	= 1024;
	dmScreenSettings.dmPelsHeight= 768;
	// 	  dmScreenSettings.dmBitsPerPel	= 32;
	// its risky to remove the flag and bits but probably safe on compo machine :-)
	dmScreenSettings.dmFields=DM_PELSWIDTH|DM_PELSHEIGHT;
	ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);  
	
	PIXELFORMATDESCRIPTOR pfd = {};
	// minimal windows setup code for opengl  
	pfd.cColorBits = pfd.cDepthBits = 32;
	pfd.dwFlags    = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;	

	// "HDC hDC" changed 19. April 2007 by benny!weltenkonstrukteur.de
	deviceContext = GetDC( CreateWindow("edit", 0, WS_POPUP|WS_VISIBLE|WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0) );

	SetPixelFormat ( hDC, ChoosePixelFormat ( hDC, &pfd) , &pfd );
	wglMakeCurrent ( hDC, wglCreateContext(hDC) );

#else
	uint32 flags = WS_VISIBLE | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
	if(state.fullscreen)
	{
		windowWidth = GetSystemMetrics(SM_CXSCREEN);
		windowHeight = GetSystemMetrics(SM_CYSCREEN);
		ShowCursor(FALSE);
		flags = WS_VISIBLE | WS_POPUP | WS_MAXIMIZE;
	}
	
	//create window
	window = CreateWindowExA(0, wc.lpszClassName, title,
		flags, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
		NULL, NULL, instance, NULL);

	//get the device context
	deviceContext = GetDC(window);

	int bytePerPixel = 4;

	//create the backbuffer
	backBuffer.info.bmiHeader.biSize = sizeof(backBuffer.info.bmiHeader);
	backBuffer.info.bmiHeader.biWidth = windowWidth;
	backBuffer.info.bmiHeader.biHeight = -windowHeight;	//if biHeight is positive the bitmap is a bottom-up DIB and it's origin is the lower-left corner,
														//if it is negative, the bitmap is a top-down DIB and it's origin is the upper-left corner.
	backBuffer.info.bmiHeader.biPlanes = 1;
	backBuffer.info.bmiHeader.biBitCount = 32;
	backBuffer.info.bmiHeader.biCompression = BI_RGB;

	int bitmapMemorySize = (windowWidth * windowHeight) * bytePerPixel;

	backBuffer.width = windowWidth;
	backBuffer.height = windowHeight;
	
	//allocates memory
	backBuffer.memory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
	//pitch, how many bytes a pointer has to move to go from one row to the next 
	backBuffer.pitch = windowWidth * bytePerPixel;

	pixels = (uint32 *)backBuffer.memory;
#endif
	initUpdate();
	randomSeed(GetTickCount());
}

void quit()
{
	shutdown();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
//Check for memory leak if debug build
/*#if defined(_DEBUG) || defined(FRAMEWORK_INTERNAL)     
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif*/	
	//minimum timer resolution, in ms, for the application or device driver. lower value = more accurate resolution.
	//set the windows scheduler (all the processing threads) granularity to 1ms so that our Sleep() can be more granular.
	instance = hInstance;
	UINT desiredSchedulerMS = 1;
	bool32 sleepIsGranual = (timeBeginPeriod(desiredSchedulerMS) == TIMERR_NOERROR);

	setup();

	MSG	msg = {};
	LARGE_INTEGER lastCounter;
	QueryPerformanceCounter(&lastCounter);

	//generates the rdtsc instruction, which returns the processor time stamp. 
	//the processor time stamp records the number of clock cycles since the last reset.
	uint64 lastCycleCount = __rdtsc();
	
	int monitorRefreshHz = 60;
	HDC refreshDC = GetDC(window);
	int win32RefreshRate = GetDeviceCaps(refreshDC, VREFRESH);
	ReleaseDC(window, refreshDC);
	if (win32RefreshRate > 1)
	{
		monitorRefreshHz = win32RefreshRate;
	}
	real32 gameUpdateHz = (monitorRefreshHz / 2.0f);
	real32 targetSecondsPerFrame = 1.0f / (real32)gameUpdateHz;

	//this is the time it takes to draw a frame
	//this allows us to run the game in any framerate we want
	deltaTime = targetSecondsPerFrame;
	
	//main loop
	bool32 running = true;
	while (running)
	{
		while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				running = false;

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		//TODO: fix timedelta

		winDimension = getWindowDimension();
		updateInput();

		#ifdef OPENGL_WIN32
			glLoadIdentity();
		#endif

		updateAndDraw();
		
		#ifdef WIN_32
			uploadPixels();
		#endif		
		
		#if defined(OPENGL_WIN32)
			SwapBuffers(deviceContext);
		#endif
		
		LARGE_INTEGER workCounter;
		QueryPerformanceCounter(&workCounter);
		real32 workSecondsElapsed = win32GetSecondsElapsed(lastCounter, workCounter);

		real32 secondsElapsedForFrame = workSecondsElapsed;
		//lock rendering to desired fps
		if (secondsElapsedForFrame < targetSecondsPerFrame)
		{
			if (sleepIsGranual)
			{
				//ms left to wait
				DWORD sleepMS = (DWORD)(1000.0f * (targetSecondsPerFrame - secondsElapsedForFrame));
				if (sleepMS > 0)
					Sleep(sleepMS);
			}

			//check so we didn't sleep to long
			//real32 testSecondsElapsedForFrame = win32GetSecondsElapsed(lastCounter, win32GetWallClock());
			//Assert(testSecondsElapsedForFrame < targetSecondsPerFrame);

			while (secondsElapsedForFrame < targetSecondsPerFrame)
			{
				LARGE_INTEGER tempCounter;
				QueryPerformanceCounter(&tempCounter);
				secondsElapsedForFrame = win32GetSecondsElapsed(lastCounter, tempCounter);
			}
		}
		else
		{
			/*TODO: Missed frame rate
			Logging */
		}

		uint64 endCycleCount = __rdtsc();
		LARGE_INTEGER endCounter;
		QueryPerformanceCounter(&endCounter); //LARGE:INTEGER is a union, the start time is in beginCounter.QuadPart;
		

	#if defined(FRAMEWORK_INTERNAL) 
			uint64 cyclesElapsed = endCycleCount - lastCycleCount;
			int64 counterElapsed = endCounter.QuadPart - lastCounter.QuadPart;
			//dividing counter/freq will give you a number of seconds since some unknown time in the past.
			real64 msPerFrame = (1000.0f*(real64)counterElapsed / (real64)state.performanceFrequency);
			real64 fps = (real64)state.performanceFrequency / (real64)counterElapsed;// -lastCounter)
			real64 megaCyclesPerFrame = (real64)(cyclesElapsed / (1000.0f * 1000.0f));
			//fps*megacycles / 1000 = processor speed
			char textBuffer[256];
			_snprintf_s(textBuffer, sizeof(textBuffer),
				"%.03fms, %.03fFPS, %.03fMEGAc/f, winWidth = %d, winHeight = %d, mouseX = %d, mouseY = %d, LMB = %d, MMB = %d, RMB = %d",
				msPerFrame, fps, megaCyclesPerFrame, windowWidth, windowHeight, mouseX, mouseY, mouseButtons[MOUSE_LEFT], mouseButtons[MOUSE_MIDDLE], mouseButtons[MOUSE_RIGHT]);
			//setColor(0, 255, 0);
			//setColor(Color::cornflowerblue);
		//	print(textBuffer, 1, windowHeight - 60);
			//OutputDebugStringA(message);
	#endif
		lastCounter = endCounter;
		lastCycleCount = endCycleCount;
	}

	shutdown();
	//release the device context
	ReleaseDC(window, deviceContext);
	//release memory 
	VirtualFree(backBuffer.memory, 0, MEM_RELEASE);
	return 0;
}
