	#define WIN32_LEAN_AND_MEAN				//#define WIN32_LEANER_AND_MEANER //#define VC_EXTRALEAN
	#pragma comment(lib, "user32.lib")
	#pragma comment(lib, "gdi32.lib")
	//#pragma comment(lib, "winmm.lib") 
	#pragma comment(lib, "Xinput9_1_0.lib") //Xinput.lib

	#include <Windows.h>
	#include <Xinput.h>
	#include "creativeframework.h"

	HWND		hWnd;						//handle to the window.
	HINSTANCE	instance;					//handle to this application
	HDC			deviceContext = NULL;		//GDI Device Context
	TEXTMETRIC	tm;							//system font

	static int xOffset;
	static int yOffset;
	
	struct screenBuffer
	{
		BITMAPINFO info;					//backbuffer bitmap info structer
		void *memory;						//pixel memory
		int pitch;
	};

	static screenBuffer backBuffer;

static inline void _pixel(int32 x, int32 y)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	SetPixel(deviceContext, x, y, RGB(color.r, color.g, color.b));
}

static inline void _pixel(int32 x, int32 y, uint32 col)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	uint8 r = (col & 0b00000000111111110000000000000000) >> 16;
	uint8 g = (col & 0b00000000000000001111111100000000) >> 8;
	uint8 b = (col & 0b00000000000000000000000011111111);
	SetPixel(deviceContext, x, y, RGB(r, g, b));
}

static inline void _pixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	SetPixel(deviceContext, x, y, RGB(r, g, b));
}

static inline void _pixel(int32 x, int32 y, ColorRGB col)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	SetPixel(deviceContext, x, y, RGB(col.r, col.g, col.b));
}

inline void _line(int32 x0, int32 y0, int32 x1, int32 y1)
{
	//create a colored pen
	HPEN hpen = CreatePen(PS_SOLID, 1, RGB(color.r, color.g, color.b));

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


inline void _circle(int32 x0, int32 y0, int32 radius)
{
	//create the pens and brushes
	HPEN   pen = CreatePen(PS_SOLID, 1, RGB(color.r, color.g, color.b));
	//HBRUSH brush = CreateSolidBrush(RGB(color.r, color.g, color.b));
	SelectObject(deviceContext, pen);

	if (fillFlag)
	{
		HBRUSH brush = CreateSolidBrush(RGB(color.r, color.g, color.b));
		SelectObject(deviceContext, brush);
		Ellipse(deviceContext, x0, y0, x0 + radius, y0 + radius);
		DeleteObject(brush);
	}
	else
	{
		//TODO: get this to work!
		Ellipse(deviceContext, x0, y0, x0 + radius, y0 + radius);
	}
	DeleteObject(pen);
}

static inline void uploadPixels()
{
	//rect to rect copy,copy from one buffer to another and stretches it if needed to fit the destination
	StretchDIBits(deviceContext,
		0, 0, screenWidth, screenHeight,	//destination rect - window 
		0, 0, screenWidth, screenHeight,	//source rect - backbuffer
		backBuffer.memory,					//pointer to image bits stored in an array of bytes
		&backBuffer.info,
		DIB_RGB_COLORS,						//color format 
		SRCCOPY);							//raster-operation 
}

static inline void print(char *message, int x, int y)
{
	SetBkMode(deviceContext, OPAQUE); //TRANSPARENT
	SetTextColor(deviceContext, RGB(color.r, color.g, color.b));
	SetBkColor(deviceContext, RGB(0, 0, 0));
	TextOut(deviceContext, x, y, message, strlen(message));
}

//Message handler callback function.
//Called by DispatchMessage() when the application receives a message.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//0 = we handled the message
	LRESULT result = 0;

	//get virtual key code and data bits
	int virtualKeycode = (int)wParam;
	int keyState = (int)lParam;

	switch (message)
	{
		case WM_SIZE:
		{
			
		} break;

		case WM_KEYDOWN:
			switch (virtualKeycode)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);	//send WM_QUIT message
				break;

			case VK_SPACE:
				PostMessage(hWnd, WM_USER, 0, 0); //send WM_USER message
				break;

			case VK_RIGHT:  break;
			case VK_LEFT:  break;
			case VK_UP:  break;
			case VK_DOWN: break;
				/*#define ALT_STATE_BIT 0x20000000
				if (keyState & ALT_STATE_BIT)
				{
				// do something
				} */
			} break;

		case WM_DESTROY:	
		{
		} break;

		case WM_CLOSE:		
		{
			PostQuitMessage(0);
		} break;

		case WM_ACTIVATEAPP:
		{
		} break;

		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			deviceContext = BeginPaint(hWnd, &paint);
			EndPaint(hWnd, &paint);
		}
		case WM_MOUSEMOVE:
		{
			// get the position of the mouse
			mouseX = (int)LOWORD(lParam);
			mouseY = (int)HIWORD(lParam);
			int buttons = (int)wParam;

			//mouseButton[LEFT] = ((buttons & MK_LBUTTON) ? 1 : 0);
			//mouseButton[RIGHT] = ((buttons & MK_RBUTTON) ? 1 : 0);
			//SetCursor(LoadCursor(NULL, IDC_ARROW));
		} break;

		case WM_LBUTTONDOWN:
		{
			mouseButton[LEFT] = true;
		} break;

		case WM_LBUTTONUP:
		{
			mouseButton[LEFT] = false;
		} break;

		case WM_RBUTTONDOWN:
		{
			mouseButton[RIGHT] = true;
		} break;
		case WM_RBUTTONUP:
		{
			mouseButton[RIGHT] = false;
		} break;

		case WM_MBUTTONDOWN:
		{
			mouseButton[MIDDLE] = true;
		} break;

		case WM_MBUTTONUP:
		{
			mouseButton[MIDDLE] = false;
		} break;

		case WM_MOUSEWHEEL:
		{
			//mouseWheel += GET_WHEEL_DELTA_WPARAM(WParam) > 0 ? +1.0f : -1.0f;
			//return true;
		} break;

		case WM_MOUSELEAVE:
		{
			mouseButton[LEFT] = false;
			mouseButton[RIGHT] = false;
			mouseButton[MIDDLE] = false;
		} break;

		case WM_LBUTTONDBLCLK: break;

		//our messages
		case WM_USER:
		{
			PostQuitMessage(0);
		} break;

		default:
		{
			result = DefWindowProc(hWnd, message, wParam, lParam); //let windows take care of any unhandled messages
		} break;
	}
	return result;
}


int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	setup();

	MSG	msg = {};
	LARGE_INTEGER lastCounter;
	QueryPerformanceCounter(&lastCounter);

	//generates the rdtsc instruction, which returns the processor time stamp. 
	//the processor time stamp records the number of clock cycles since the last reset.
	uint64 lastCycleCount = __rdtsc();

	//main loop
	while (running)
	{
		while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				running = false;

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		//loop through the game controllers and get it's state
		for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; ++controllerIndex)
		{
			XINPUT_STATE controllerState;
			if (XInputGetState(controllerIndex, &controllerState) == ERROR_SUCCESS)
			{
				//controller is plugged in
				XINPUT_GAMEPAD *pad = &controllerState.Gamepad;
				bool32 up = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
				bool32 down = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
				bool32 left = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
				bool32 right = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
				bool32 start = (pad->wButtons & XINPUT_GAMEPAD_START);
				bool32 back = (pad->wButtons & XINPUT_GAMEPAD_BACK);
				bool32 leftShoulder = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
				bool32 rightShoulder = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
				bool32 aButton = (pad->wButtons & XINPUT_GAMEPAD_A);
				bool32 bButton = (pad->wButtons & XINPUT_GAMEPAD_B);
				bool32 xButton = (pad->wButtons & XINPUT_GAMEPAD_X);
				bool32 yButton = (pad->wButtons & XINPUT_GAMEPAD_Y);

				int16 stickX = pad->sThumbLX;
				int16 stickY = pad->sThumbLY;
				xOffset += stickX >> 12;
				yOffset += stickY >> 12;
				if (aButton)
				{
					yOffset++;
				}

			}
			else
			{
				//controller is not available
			}
		}

		//TODO: fix timedelta
		uint32 t = 0;

		updateAndDraw(t);
		
		uint64 endCycleCount = __rdtsc();
		LARGE_INTEGER endCounter;
		QueryPerformanceCounter(&endCounter); //LARGE:INTEGER is a union, the start time is in beginCounter.QuadPart;
		uint64 cyclesElapsed = endCycleCount - lastCycleCount;
		int64 counterElapsed = endCounter.QuadPart - lastCounter.QuadPart;
		//dividing counter/freq will give you a number of seconds since some unknown time in the past.
		real64 msPerFrame = (1000.0f*(real64)counterElapsed / (real64)performanceFrequency);
		real64 fps = (real64)performanceFrequency / (real64)counterElapsed;// -lastCounter)
		real64 megaCyclesPerFrame = (real64)(cyclesElapsed / (1000.0f * 1000.0f));
		//fps*megacycles / 1000 = processor speed

	#if defined(_DEBUG) 
			char textBuffer[256];
			int length = _snprintf_s(textBuffer, sizeof(textBuffer),
				"%.03fms, %.03fFPS, %.03fMEGAc/f, winWidth = %d, winHeight = %d, mouseX = %d, mouseY = %d, LMB = %d, MMB = %d, RMB = %d",
				msPerFrame, fps, megaCyclesPerFrame, screenWidth, screenHeight, mouseX, mouseY, mouseButton[LEFT], mouseButton[MIDDLE], mouseButton[RIGHT]);
			//setColor(0, 255, 0);
			setColor(Color::cornflowerblue);
			print(textBuffer, 1, screenHeight - 60);
			//OutputDebugStringA(message);
	#endif
		lastCounter = endCounter;
		lastCycleCount = endCycleCount;
		ReleaseDC(hWnd, deviceContext);
	}

	//release the device context
	ReleaseDC(hWnd, deviceContext);
	//release memory 
	VirtualFree(backBuffer.memory, 0, MEM_RELEASE);
	return 0;
}


void screen(int width, int height, bool32 screen, char *title)
{
	LARGE_INTEGER perfCountFrequencyResult;
	//retrieves the frequency of the performance counter in counts per seonds
	QueryPerformanceFrequency(&perfCountFrequencyResult);
	performanceFrequency = perfCountFrequencyResult.QuadPart;

	//run in windowed mode in debug, but maximized in release.
	#if defined(_DEBUG)    
		screenWidth = width;
		screenHeight = height;
		#define WINDOWFLAGS (WS_VISIBLE | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU)
	#else
		screenWidth = GetSystemMetrics(SM_CXSCREEN);	// hämtar systemets upplösning för max fönster
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
		ShowCursor(FALSE);
		#define WINDOWFLAGS (WS_VISIBLE | WS_POPUP | WS_MAXIMIZE)
	#endif

	//create and initialize the windows class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0, 0,
		instance, NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL, "win32-framework", NULL };

	//register the window class
	RegisterClassExA(&wc);

	//create window
	hWnd = CreateWindowExA(0, "win32-framework", title,
		WINDOWFLAGS, CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight,
		NULL, NULL, instance, NULL);

	//get the device context
	HDC deviceContext = GetDC(hWnd);

	int bytePerPixel = 4;

	//create the backbuffe
	backBuffer.info.bmiHeader.biSize = sizeof(backBuffer.info.bmiHeader);
	backBuffer.info.bmiHeader.biWidth = screenWidth;
	backBuffer.info.bmiHeader.biHeight = -screenHeight;	//if biHeight is positive the bitmap is a bottom-up DIB and it's origin is the lower-left corner,
														//if it is negative, the bitmap is a top-down DIB and it's origin is the upper-left corner.
	backBuffer.info.bmiHeader.biPlanes = 1;
	backBuffer.info.bmiHeader.biBitCount = 32;
	backBuffer.info.bmiHeader.biCompression = BI_RGB;

	int bitmapMemorySize = (screenWidth * screenHeight) * bytePerPixel;

	//allocates memory
	backBuffer.memory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
	//pitch, how many bytes a pointer has to move to go from one row to the next 
	backBuffer.pitch = screenWidth * bytePerPixel;

	pixelBuffer = (uint32 *)backBuffer.memory;

	srand(GetTickCount());
}

void setup()
{
	screen(960, 540, false, "win32 framework");
	fillFlag = false;
}

void updateAndDraw(uint32 t)
{
	
	/*for (int y = 0; y < screenHeight; y++)
	{
		for (int x = 0; x < screenWidth; x++)
		{
			uint8 red = (y + yOffset);
			uint8 blue = (x + xOffset);
			uint8 green = 0;// (y + yOffset);
			//uint32 *pixelBuffer = (uint32 *)backBuffer.memory;
			//pixelBuffer[y*screenWidth + x] = (red << 16) | (green << 8) | blue;
			pixel(x, y, ((red << 16) | (green << 8) | blue));
		}
	}*/
	
	int x = rand() % screenWidth;
	int y = rand() % screenHeight;

	setColor(rand() % 255, rand() % 255, rand() % 255);
	//uint32 *pixelBuffer = (uint32 *)backBuffer.memory;
	//pixelBuffer[y*screenWidth + x] = (0b11111111 << 16) | (0b00000000 << 8) | 0b11111111;
	//pixel(x, y, color);
	//circle(x, y, rand() % 100);
	line(rand() % screenWidth, rand() % screenHeight, rand() % screenWidth, rand() % screenHeight);
	//RECT rect = { 0,0,100,100 };
	//FillRect(deviceContext, &rect, (HBRUSH)CreateSolidBrush(RGB(0, 255, 0)));
	//PatBlt(deviceContext, 0, 0, screenWidth, screenHeight-100, BLACKNESS);

	//clear(Color::magenta);
	uploadPixels();
	
	//_pixel(x, y, color);
	//_line(rand() % screenWidth, rand() % screenHeight, rand() % screenWidth, rand() % screenHeight);
	//_circle(rand() % screenWidth, rand() % screenHeight, rand() % 100);
}

void shutdown() { }