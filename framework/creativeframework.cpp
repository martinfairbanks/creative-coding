	/* TODO:	scroll wheel
				check xbox controller
				rumble
	*/
	#if defined(_DEBUG) || defined(FRAMEWORK_INTERNAL)
		#define Assert(expression) if(!(expression)) { *(int *)0 = 0; }
		#define _CRT_SECURE_NO_WARNINGS
		//#define _CRT_SECURE_NO_DEPRECATE
		char debugMessage[256];
	#else
		#define Assert(expression)
	#endif

	#include <stdint.h>
	//#include <sstream>	//sprintf
	#include "dependencies\slang_library_noise.cpp"
	#define STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_WRITE_IMPLEMENTATION
	#include "dependencies/stb_image.h"
	#include "dependencies/stb_image_write.h"

	typedef int8_t int8;
	typedef int16_t int16;
	typedef int32_t int32;
	typedef int64_t int64;
	typedef int32 bool32;
	typedef uint8_t uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;
	typedef float real32;
	typedef double real64;

	#include "framework_math.cpp"
	#include "framework_strings.cpp"

	//LL = make it a 64bit number, otherwise the value will wrap when we ask for 4 gigs of memory
	#define Kilobytes(value) ((value)*1024LL)
	#define Megabytes(value) (Kilobytes(value)*1024LL)
	#define Gigabytes(value) (Megabytes(value)*1024LL)
	#define Terabytes(value) (Gigabytes(value)*1024LL)
	#define arrayCount(a) (sizeof(a) / sizeof(a[0]))

	#define arrayColumns(name)    \
		(sizeof(name[0]) / sizeof(name[0][0]))
	#define arrayRows(a)       \
		(sizeof(a) / sizeof(a[0]))


	/* Globals 
	   these variables is globals to mimic the p5.js variables
	*/
	enum ColorModes
	{
		RGB,
		HSB,
		HSL
	};

	int32 windowWidth = 0;
	int32 windowHeight = 0; 
	real32 deltaTime = 0;					//time/frame
	uint64 frameCount = 0;					//number of frames drawn since program start
	int32 framesPerSecond = 0;
	uint32 *pixels = 0;						//pixel buffer for pixel manipulation
	vec2 center{0, 0};						//center location in window

	bool32 mouseButtons[4];
	bool32 prevMouseButtons[4];
	int32 mouseX;
	int32 mouseY;
	int32 prevMouseX;
	int32 prevMouseY;
	bool32 mouseDragged = false;
	bool32 mouseMoved = false;

	const real32 stickDeadZone = 7849.0f;
#if defined(SDL2)
	#define MAX_KEYS 512
#else
	#define MAX_KEYS 256
#endif
	bool32 keys[MAX_KEYS];
	bool32 prevKeys[MAX_KEYS];
	
	struct State
	{
		bool32 vSync = true;
		bool32 fullscreen = false;
		bool32 fillFlag = true;				//fill flag for shapes
		bool32 softwareRenderer = true;
		int32 lineWidth = 1;				//strokeweight	
		int32 colorModeFlag = RGB;
		bool32 running;
		
		uint64 performanceFrequency = 0;	//the frequency of the performance counter in counts per seonds

		uint64 totalMemorySize;
		void *memoryBlock;

		//UI state
		int32 hotWidget;					//widget is below the mouse cursor
		int32 activeWidget;					//user is interacting with the widget
		int32 keyboardFocus;
		int32 keyentered;
		int32 keymod;
		int32 keychar;
		int32 lastwidget;
	} state;

bool32 keyDown(int32 key)
{
	if (key < 0 || key > MAX_KEYS)
		return false;

	return keys[key];
}

bool32 keyHit(int32 key)
{
	if (key < 0 || key > MAX_KEYS)
		return false;

	return (keys[key] && !prevKeys[key]);
}

bool32 keyUp(int32 key)
{
	if (key < 0 || key > MAX_KEYS)
		return false;

	return (prevKeys[key] && !keys[key]);
}

bool32 mouseDown(int32 key = 1)
{
	if (key < 0 || key > 3)
		return false;

	return mouseButtons[key];
}

bool32 mouseClick(int32 key = 1)
{
	if (key < 0 || key > 3)
		return false;

	return (mouseButtons[key] && !prevMouseButtons[key]);
}

bool32 mouseUp(int32 key = 1)
{
	if (key < 0 || key > 3)
		return false;

	return (prevMouseButtons[key] && !mouseButtons[key]);
}

real32 processStickValue(real32 value, real32 deadZoneThreshold)
{
	//return 0 if we got a value that are in the dead zone 
	real32 result = 0;

	if(value < -deadZoneThreshold)
	{
		//NOTE(martin):if we say that the deadszone ends at 0.2, 
		//this will map the value to 0 - 1 instead of 0.2 - 1
		result = (value + deadZoneThreshold) / (32768.0f - deadZoneThreshold);
	}
	else if(value > deadZoneThreshold)
	{
		result = (value - deadZoneThreshold) / (32767.0f - deadZoneThreshold);
	}

	return result;
}

/* File I/O */
//remember to free the memory after you called this function!
char* loadTextFile(char* filename)
{
	char *contents = 0;
	FILE *file = fopen(filename, "r");

	if (file)
	{
		fseek(file, 0, SEEK_END);				//go to end of file
		size_t fileSize = ftell(file);			//get file size
		fseek(file, 0, SEEK_SET);				//go to beginning of file
		contents = (char *)malloc(fileSize);	//allocate memory for file + 1 byte for null terminator

		//clear memory
		for (int i = 0; i < fileSize + 1; i++) {
			contents[i] = 0;
		}

		fread(contents, fileSize, 1, file);		//read in the file
		contents[fileSize] = 0;					//insert the null terminator

		fclose(file);
	}
	return contents;
}

void* loadFile(char* filename)
{
	FILE* fp = fopen(filename, "r");
	//move pointer to the end of the file to get length
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	void *contents = (void *)malloc((fileSize) * sizeof(char));

	//read file
	fread(contents, 1, fileSize, fp);
	fclose(fp);
	return contents;
}

	union Color
	{
		struct
		{
			int32 r, g, b, a;
		};
		struct
		{
			int32 h, s, b, a;
		};
		struct
		{
			int32 h, s, l, a;
		};
		struct
		{
			real32 fR, fG, fB, fA;
		};
		
		int32 e[4];	
	};

	union Colorf
	{
		struct
		{
			real32 r, g, b, a;
		};
		struct
		{
			real32 h, s, b, a;
		};
		struct
		{
			real32 h, s, l, a;
		};

		real32 e[4];
	};

#if defined(SDL2) || defined(WIN_32)
	Color strokeColor{ 255, 255, 255, 255 };
	Color fillColor{ 255, 255, 255, 255 };
#else
	Colorf strokeColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	Colorf fillColor{ 0.0f, 0.0f, 0.0f, 1.0f };
#endif

	Color black{ 0,0,0, 255 };
	Color white{ 255,255,255, 255 };
	Color c64red{ 104, 55, 43, 255 };
	Color c64cyan{ 112, 164, 178, 255 };
	Color c64purple{ 111, 61, 134, 255 };
	Color c64green{ 88, 141, 67, 255 };;
	Color c64blue{ 53, 40, 121, 255 };
	Color c64yellow{ 184, 199, 111, 255 };
	Color c64orange{ 111, 79, 37, 255 };;
	Color c64brown{ 67, 57, 0, 255 };
	Color c64lightred{ 154, 103, 89, 255 };
	Color c64darkgrey{ 68, 68, 68, 255 };
	Color c64grey{ 108, 108, 108, 255 };
	Color c64lightgreen{ 154, 210, 132, 255 };
	Color c64lightblue{ 108, 94, 181, 255 };
	Color c64lightgrey{ 149, 149, 149, 255 };
	Color red{ 255,0,0, 255 };
	Color green{ 0,255,0, 255 };;
	Color blue{ 0,0,255, 255 };
	Color yellow{ 255,255,0, 255 };
	Color cyan{ 0,255,255, 255 };
	Color magenta{ 255,0,255, 255 };
	Color purple{ 128, 0, 128, 255 };
	Color gray{ 128, 128, 128, 255 };
	Color grey{ 192, 192, 192, 255 };
	Color maroon{ 128, 0, 0, 255 };
	Color darkgreen{ 0, 128, 0, 255 };
	Color navy{ 0, 0, 128, 255 };
	Color teal{ 0, 128, 128, 255 };
	Color olive{ 128, 128, 0, 255 };
	Color orange{ 255,127,50, 255 };
	Color cornflowerblue{ 101, 156, 239, 255 };
	Color azure{ 0, 127, 255, 255 };
	Color turquoise{ 48, 213, 200, 255 };
	Color gold{ 255, 215, 0, 255 };
	Color silver{ 192, 192, 192, 255 };
	Color pink{ 255, 192, 203, 255 };
	Color brown = { 0xa5,0x2a,0x2a, 0xff };


void setup();
void updateAndDraw();
void shutdown();
void quit();

void colorMode(int32 mode)
{
	switch (mode)
	{
	case RGB:
		state.colorModeFlag = RGB;
		break;
	case HSB:
		state.colorModeFlag = HSB;
		break;
	case HSL:
		state.colorModeFlag = HSL;
		break;
	}
}

//converts HSB(=HSV) color to RGB color
Color colorHSB(Color colorHSB)
{
	if (colorHSB.r < 0 || colorHSB.r > 255)
		colorHSB.r = 0;
	if (colorHSB.g < 0 || colorHSB.g > 255)
		colorHSB.g = 0;
	if (colorHSB.b < 0 || colorHSB.b > 255)
		colorHSB.b = 0;

	real32 r=0, g=0, b=0, h, s, v;
	h = colorHSB.r / 256.0f;
	s = colorHSB.g / 256.0f;
	v = colorHSB.b / 256.0f;

	if (s == 0.0)
		r = g = b = v;
	else
	{
		real32 f, p, q, t;
		int32 i;
		h *= 6.0;
		i = int32(floor(h));
		f = h - i;
		p = v * (1.0f - s);
		q = v * (1.0f - (s * f));
		t = v * (1.0f - (s * (1.0f - f)));

		switch (i)
		{
			case 0: r = v; g = t; b = p; break;
			case 1: r = q; g = v; b = p; break;
			case 2: r = p; g = v; b = t; break;
			case 3: r = p; g = q; b = v; break;
			case 4: r = t; g = p; b = v; break;
			case 5: r = v; g = p; b = q; break;
		}
	}
	Color col;
	col.r = int32(r * 255.0f);
	col.g = int32(g * 255.0f);
	col.b = int32(b * 255.0f);
	col.a = 255;
	return col;
}

//converts HSL color to RGB color
Color colorHSL(Color colorHSL)
{
	real32 r, g, b, h, s, l;
	real32 tempCol1, tempCol2, tempr, tempg, tempb;
	h = (real32)colorHSL.r / 256.0f;
	s = (real32)colorHSL.g / 256.0f;
	l = (real32)colorHSL.b / 256.0f;

	if (s == 0)
		r = g = b = l;
	else
	{
		if (l < 0.5)
			tempCol2 = l * (1 + s);
		else
			tempCol2 = (l + s) - (l * s);
		
		tempCol1 = 2 * l - tempCol2;
		tempr = h + 1.0f / 3.0f;
		
		if (tempr > 1) 
			tempr--;
		
		tempg = h;
		tempb = h - 1.0f / 3.0f;
		
		if (tempb < 0)
			tempb++;
		
		//red
		if (tempr < 1.f / 6.f)
			r = tempCol1 + (tempCol2 - tempCol1) * 6.f * tempr;
		else if (tempr < 0.5f)
			r = tempCol2;
		else if (tempr < 2.f / 3.f)
			r = tempCol1 + (tempCol2 - tempCol1) * ((2.f / 3.f) - tempr) * 6.f;
		else
			r = tempCol1;

		//green
		if (tempg < 1.f / 6.f)
			g = tempCol1 + (tempCol2 - tempCol1) * 6.f * tempg;
		else if (tempg < 0.5) 
			g = tempCol2;
		else if (tempg < 2.f / 3.f)
			g = tempCol1 + (tempCol2 - tempCol1) * ((2.f / 3.f) - tempg) * 6.f;
		else 
			g = tempCol1;

		//blue
		if (tempb < 1.0 / 6.0) 
			b = tempCol1 + (tempCol2 - tempCol1) * 6.f * tempb;
		else if (tempb < 0.5)
			b = tempCol2;
		else if (tempb < 2.f / 3.f)
			b = tempCol1 + (tempCol2 - tempCol1) * ((2.f / 3.f) - tempb) * 6.f;
		else 
			b = tempCol1;
	}

	Color col;
	col.r = int32(r * 255.f);
	col.g = int32(g * 255.f);
	col.b = int32(b * 255.f);
	col.a = 255;
	return col;
}

Color checkColorMode(int32 r, int32 g, int32 b)
{
	Color col = { r, g, b, 255 };
	Color tempCol;
	
	if (state.colorModeFlag == HSB)
		tempCol = colorHSB(col);
	else if (state.colorModeFlag == HSL)
		tempCol = colorHSL(col);
	else //RGB
		tempCol = col;

	return tempCol;
}

void printLogError(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("Error: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
	system("PAUSE");
	quit();
	exit(1);
}

void printLog(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("Log: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

/* ----- Data Structures ----- */
// TODO: linked list, hash table, binary tree
struct Entity;
#include <vector>
struct VecArray
{
	std::vector<Entity*> entity;

	size_t length = 0;
	void push(Entity *ent)
	{
		//data.push_back(ent);
		entity.emplace_back(ent);
		length = entity.size();
	}

	void splice(int32 start, int32 end)
	{
		//if (!data.empty())
		//{
		/*for (int32 i = start; i <= end; i++)
		{
		delete data[i];
		}*/
		delete entity[start];
		entity.erase(entity.begin() + start, entity.begin() + start + end);
		length = entity.size();
		//}
	}
	void reserve(int32 val)
	{
		entity.reserve(val);
	}
	void free()
	{
		for (auto &a : entity)
		{
			delete a;
		}
		//data.shrink_to_fit();
	}
};

//dynamic array
struct Array
{
	int32 length;
	int32 size;
	void **data;

	void reserve(int32 initSize)
	{
		length = 0;
		size = initSize;
		data = (void**)malloc(sizeof(void *) * size);
	}

	void resize(int32 newSize)
	{

#if defined(_DEBUG) || defined(FRAMEWORK_INTERNAL)
		printf("Realloc the Array: %d to %d\n", size, newSize);
#endif
		void **newArray = (void**)realloc(data, sizeof(void *) * newSize);
		if (newArray)
		{
			data = newArray;
			size = newSize;
		}
	}

	void push(void *value)
	{
		if (size == length)
			resize(size * 2);
		data[length++] = value;
	}

	void splice(int32 index)
	{
		if (index < 0 || index >= length)
			return;

		free(data[index]);
		//delete data[index];
		data[index] = 0;

		for (int i = index; i < length - 1; i++)
		{
			data[i] = data[i + 1];
			data[i + 1] = 0;
		}

		length--;

		if (length > 0 && length == size / 4)
			resize(size / 2);
	}

	void set(int index, void *value)
	{
		if (index >= 0 && index < length)
			data[index] = value;
	}

	void *get(int index)
	{
		if (index >= 0 && index < length)
			return data[index];
		return 0;
	}

	void freeMem()
	{
		for (int32 i = 0; i < length; i++) {
			free(data[i]);
		}
		free(data);
	}

};


#if defined(SDL2)
	#include "sdl2_functions_shared.cpp"
	#include "sdl2_functions.cpp"
	#include "sdl2_framework.cpp"
#endif

#if defined(WIN_32) || defined(OPENGL_WIN32)
	#include "win32_framework.cpp"
#endif

#if defined(OPENGL)
	#include "sdl2_functions_shared.cpp"
	#include "opengl_functions.cpp"
	#include "sdl2_framework.cpp"
#endif

//#if defined(OPENGL_WIN32)
//	#include "win32_opengl_framework.cpp"
//#endif

#if defined(OPENGL4)
	#include "sdl2_functions_shared.cpp"
	#include "opengl4_functions.cpp"
	#include "sdl2_framework.cpp"
#endif


#if defined(SDL2) || defined(WIN_32)
	#include "software_renderer.cpp"
#endif
