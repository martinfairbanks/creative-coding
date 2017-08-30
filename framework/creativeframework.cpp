#include <stdint.h>
#include <stdlib.h> //srand
#include <crtdbg.h> //memory leak check
#include <math.h>
#include <float.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float float32;
typedef double float64;
//#define _CRT_SECURE_NO_DEPRECATE
#define PI32 3.14159265359f
#define SWAP(x, y)\
		x = x + y; \
		y = x - y; \
		x = x - y;

#if defined(_DEBUG)
#include <iostream>
#define LOG(x) std::cout << x << std::endl;
#define LOGERROR(x)\
				std::cout << x << std::endl; \
				system("PAUSE"); \
				SDL_Quit(); \
				exit(1);
#define Assert(expression) if(!(expression)) { *(int *)0 = 0; }
#else
	#define LOG(x) 
	#define LOGERROR(x)
	#define Assert(expression)
#endif

int32 screenWidth;
int32 screenHeight; 
bool32 fullscreen = false;
bool32 running = true;
bool32 vSync = true;
uint64 performanceFrequency;		//the frequency of the performance counter in counts per seonds
uint32 *pixelBuffer;				//pixelbuffer for pixel manipulation
bool32 fillFlag = true;				//fill flag for shapes
int32 lineWidth = 1;				//strokeweight

void setup();
void updateAndDraw(uint32 t);
void shutdown();

/* ----- Input ----- */
int32 mouseX = 0;
int32 mouseY = 0;
uint32 mouseButton[4] = {};

bool32 mouseIsPressed = false;	//true if mouse is pressed
//bool32 mouseClick = false;
bool32 mouseReleased = false;

enum MouseButtons
{
	LEFT = 1,
	MIDDLE,
	RIGHT
};

/* ----- Math ----- */
struct vec2
{
	float32 x;
	float32 y;
	vec2() { };
	vec2(float32 X, float32 Y) : x(X), y(Y) {}

	//overloaded operator for vector addition: v3 = v1 + v2;
	vec2 operator+(const vec2& v2) const
	{
		return vec2(x + v2.x, y + v2.y);
	}

	//v1 += v2;
	friend vec2& operator+=(vec2& v1, const vec2& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		return v1;
	}

	//subtracting two vectors
	vec2 operator-(const vec2& v2) const
	{
		return vec2(x - v2.x, y - v2.y);
	}

	friend vec2& operator-=(vec2& v1, const vec2& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		return v1;
	}

	//multiply by a scalar number
	vec2 operator*(float32 scalar)
	{
		return vec2(x * scalar, y * scalar);
	}

	vec2& operator*=(float32 scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	//divide by a scalar number
	vec2 operator/(float32 scalar)
	{
		return vec2(x / scalar, y / scalar);
	}

	vec2& operator/=(float32 scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	//calculate length (magnitude) of the vector, square root of x^2 + y^2
	//The Pythagorean Theorem
	float32 length() { return sqrt(x * x + y * y); }

	//calculate the angle
	float32 angle() { return atan2(y, x); }

	//set length of vector
	void setMag(float32 length)
	{
		float32 a = angle();
		x = cos(a) * length;
		y = sin(a) * length;
	}

	void setAngle(float32 angle)
	{
		float32 l = length();
		x = cos(angle) * l;
		y = sin(angle) * l;
	}

	//limit the magnitude the vector
	void limit(float32 max)
	{
		if (length() > max)
			setMag(max);
	}

	//normalizing a vector makes its length equal to 1.
	//to normalize a vector - multiply it by the inverse of its length
	void normalize()
	{
		float32 l = length();
		if (l > 0) //avoid divide by 0
		{
			(*this) *= 1 / l;
		}
	}
};

struct vec3
{
	float32 x;
	float32 y;
	float32 z;

	vec3() { };
	vec3(float32 X, float32 Y, float32 Z) : x(X), y(Y), z(Z) {}
	//overloaded operator for vector addition: v3 = v1 + v2;
	vec3 operator+(const vec3& v2) const
	{
		return vec3(x + v2.x, y + v2.y, z + v2.z);
	}

	//v1 += v2;
	friend vec3& operator+=(vec3& v1, const vec3& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
		return v1;
	}

	//subtracting two vectors
	vec3 operator-(const vec3& v2) const
	{
		return vec3(x - v2.x, y - v2.y, z - v2.z);
	}

	friend vec3& operator-=(vec3& v1, const vec3& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;
		return v1;
	}

	//multiply by a scalar number
	vec3 operator*(float32 scalar)
	{
		return vec3(x * scalar, y * scalar, z * scalar);
	}

	vec3& operator*=(float32 scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	//divide by a scalar number
	vec3 operator/(float32 scalar)
	{
		return vec3(x / scalar, y / scalar, z / scalar);
	}

	vec3& operator/=(float32 scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	//calculate length (magnitude) of the vector, square root of x^2 + y^2
	//The Pythagorean Theorem
	float32 length() { return sqrt(x * x + y * y); }

	//normalizing a vector makes its length equal to 1.
	//to normalize a vector - multiply it by the inverse of its length
	void normalize()
	{
		float32 l = length();
		if (l > 0) //avoid divide by 0
		{
			(*this) *= 1 / l;
		}
	}
};


inline int32 random(int32 min, int32 max)
{
	/* ex:  Random(-10,20) -> will give -10 to, and including, 20. */
	max += 1;
	min -= 1;
	return ((double)rand() / (RAND_MAX + 1) * (max - min) + min);
}

inline int32 random(int32 max)
{
	int32 min = -1;
	max += 1;
	return ((double)rand() / (RAND_MAX + 1) * (max - min) + min);
}

//random float between 0 and 1
inline float32 randomf()
{
	return ((float)rand() / (RAND_MAX));
	//random float between -1 and 1
	//return (((float)rand() / (RAND_MAX)) * 2 - 1.0f);
}

inline float32 randomf(float32 min, float32 max)
{
	return min + randomf() * (max - min);
}

//random 2d direction
inline vec2 random2d()
{
	vec2 vec{ 1.0, 1.0 };
	//vec.normalize();
	vec.setAngle(randomf() * PI32 * 2);
	return vec;
}

//convert radians to degrees
float degrees(float32 radians)
{

	float degrees = (radians / (2 * PI32)) * 360;
	return degrees;
}

//convert degrees to radians
float radians(float32 degrees)
{
	float radian = (degrees / 360) * 2 * PI32;
	return radian;
}

//calculates the distance between 2 points
double dist(int32 x1, int32 y1, int32 x2, int32 y2)
{
	return sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));
}

int32 constrain(int32 value, int32 min, int32 max)
{
	return (value < min) ? min : ((value > max) ? max : value);
}

float32 constrainf(float32 value, float32 min, float32 max)
{
	return (value < min) ? min : ((value > max) ? max : value);
}

float32 mapVal(float32 value, float32 inMin, float32 inMax, float32 outMin, float32 outMax)
{
	if (fabs(inMin - inMax) < FLT_EPSILON)
	{
		return outMin;
	}
	else
	{
		float32 outVal = ((value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin);

		if (outMax < outMin)
		{
			if (outVal < outMax)outVal = outMax;
			else if (outVal > outMin)outVal = outMin;
		}
		else
		{
			if (outVal > outMax)outVal = outMax;
			else if (outVal < outMin)outVal = outMin;
		}
		
		return outVal;
	}
}

#if defined(SDL2)
	#include "sdl2_framework.cpp"
#endif

#if defined(WIN_32)
	#include "win32_framework.cpp"
#endif

#if defined(OPENGL) | defined(OPENGL4)
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

struct ColorRGBA
{
	float32 r;
	float32 g;
	float32 b;
	float32 a;
} color{ 1.0f, 1.0f, 1.0f, 1.0f };

ColorRGBA strokeColor{ 0.0f, 0.0f, 0.0f, 1.0f };
ColorRGBA fillColor{ 0.0f, 0.0f, 0.0f, 1.0f };

static ColorRGBA colorfromhex(const unsigned int hex)
{
	return ColorRGBA{ float32((hex >> 16) & 0xFF) / 255.0f, float32((hex >> 8) & 0xFF) / 255.0f, float32((hex >> 0) & 0xFF) / 255.0f, 1.0f };
}

const ColorRGBA black{ 0.0f, 0.0f, 0.0f, 1.0f };
const ColorRGBA white{ 1.0f, 1.0f, 1.0f, 1.0f };
const ColorRGBA red{ 1.0f, 0.0f, 0.0f, 1.0f };
const ColorRGBA green{ 0.0f , 1.0f, 0.0f, 1.0f };
const ColorRGBA blue{ 0.0f, 0.0f, 1.0f, 1.0f };
const ColorRGBA yellow{ 1.0f, 1.0f, 0.0f, 1.0f };
const ColorRGBA cyan{ 0.0f, 1.0f, 1.0f, 1.0f };
const ColorRGBA magenta{ 1.0f, 0.0f, 1.0f, 1.0f };
const ColorRGBA cornflowerblue = colorfromhex(0x6495ED);
const ColorRGBA azure = colorfromhex(0xF0FFFF);
const ColorRGBA turquoise = colorfromhex(0x40E0D0);
const ColorRGBA gold = colorfromhex(0xFFD700);
const ColorRGBA silver = colorfromhex(0xC0C0C0);
const ColorRGBA pink = colorfromhex(0xFFC0CB);
const ColorRGBA teal = colorfromhex(0x008080);
#endif

#if defined(OPENGL)
	#include "opengl_framework.cpp"
#endif

#if defined(OPENGL4)
	#include "opengl4_framework.cpp"
#endif

#if defined(SDL2) | defined(WIN_32)
//converts HSB(=HSV) color to RGB color
Color colorHSB(Color colorHSB)
{
	float32 r, g, b, h, s, v;
	h = colorHSB.r / 256.0;
	s = colorHSB.g / 256.0;
	v = colorHSB.b / 256.0;

	if (s == 0.0) r = g = b = v;
	else
	{
		float32 f, p, q, t;
		int32 i;
		h *= 6.0;
		i = int32(floor(h));
		f = h - i;
		p = v * (1.0 - s);
		q = v * (1.0 - (s * f));
		t = v * (1.0 - (s * (1.0 - f)));

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
	col.r = int32(r * 255.0);
	col.g = int32(g * 255.0);
	col.b = int32(b * 255.0);
	return col;
}

Color colorHSL(Color colorHSL)
{
	float32 r, g, b, h, s, l;
	float32 temp1, temp2, tempr, tempg, tempb;
	h = (float32)colorHSL.r / 256.0f;
	s = (float32)colorHSL.g / 256.0f;
	l = (float32)colorHSL.b / 256.0f;

	if (s == 0) r = g = b = l;
	else
	{
		if (l < 0.5) temp2 = l * (1 + s);
		else temp2 = (l + s) - (l * s);
		temp1 = 2 * l - temp2;
		tempr = h + 1.0 / 3.0;
		if (tempr > 1) tempr--;
		tempg = h;
		tempb = h - 1.0 / 3.0;
		if (tempb < 0) tempb++;

		//red
		if (tempr < 1.0 / 6.0) r = temp1 + (temp2 - temp1) * 6.0 * tempr;
		else if (tempr < 0.5) r = temp2;
		else if (tempr < 2.0 / 3.0) r = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
		else r = temp1;

		//green
		if (tempg < 1.0 / 6.0) g = temp1 + (temp2 - temp1) * 6.0 * tempg;
		else if (tempg < 0.5) g = temp2;
		else if (tempg < 2.0 / 3.0) g = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
		else g = temp1;

		//blue
		if (tempb < 1.0 / 6.0) b = temp1 + (temp2 - temp1) * 6.0 * tempb;
		else if (tempb < 0.5) b = temp2;
		else if (tempb < 2.0 / 3.0) b = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;
		else b = temp1;
	}

	Color col;
	col.r = int32(r * 255.0);
	col.g = int32(g * 255.0);
	col.b = int32(b * 255.0);
	return col;
}
/*
void setColor(int32 r, int32 g, int32 b)
{
	if (colorModeFlag == RGB)
	{
		color.r = r;
		color.g = g;
		color.b = b;
	}
	else if (colorModeFlag == HSB)
	{
		color = colorHSB(Color{ r, g, b });
	}
	else if (colorModeFlag == HSL)
	{
		color = colorHSL(Color{ r, g, b });
	}
}

void setColor(Color col)
{
	if (colorModeFlag == RGB)
	{
		color = col;
	}
	else if(colorModeFlag == HSB)
	{
		color = colorHSB(Color{ col.r, col.g, col.b });
	}
	else if (colorModeFlag == HSL)
	{
		color = colorHSL(Color{ col.r, col.g, col.b });
	}
}

void setColor(uint8 col)
{
	if (colorModeFlag == RGB)
	{
		color.r = col;
		color.g = col;
		color.b = col;
	}
	else if (colorModeFlag == HSB)
	{
		color = colorHSB(Color{ col, col, col });
	}
	else if (colorModeFlag == HSL)
	{
		color = colorHSL(Color{ col, col, col });
	}
}
*/
void fill(int32 r, int32 g, int32 b)
{
	if (colorModeFlag == RGB)
	{
		fillColor.r = r;
		fillColor.g = g;
		fillColor.b = b;
	}
	else if (colorModeFlag == HSB)
	{
		fillColor = colorHSB(Color{ r, g, b });
	}
	else if (colorModeFlag == HSL)
	{
		fillColor = colorHSL(Color{ r, g, b });
	}
}

void fill(Color col)
{
	if (colorModeFlag == RGB)
	{
		fillColor = col;
	}
	else if (colorModeFlag == HSB)
	{
		fillColor = colorHSB(Color{ col.r, col.g, col.b });
	}
	else if (colorModeFlag == HSL)
	{
		fillColor = colorHSL(Color{ col.r, col.g, col.b });
	}
}

void fill(uint8 col)
{
	if (colorModeFlag == RGB)
	{
		fillColor.r = col;
		fillColor.g = col;
		fillColor.b = col;
	}
	else if (colorModeFlag == HSB)
	{
		fillColor = colorHSB(Color{ col, col, col });
	}
	else if (colorModeFlag == HSL)
	{
		fillColor = colorHSL(Color{ col, col, col });
	}
}


void stroke(int32 r, int32 g, int32 b)
{
	if (colorModeFlag == RGB)
	{
		strokeColor.r = r;
		strokeColor.g = g;
		strokeColor.b = b;
	}
	else if (colorModeFlag == HSB)
	{
		strokeColor = colorHSB(Color{ r, g, b });
	}
	else if (colorModeFlag == HSL)
	{
		strokeColor = colorHSL(Color{ r, g, b });
	}
}

void stroke(Color col)
{
	if (colorModeFlag == RGB)
	{
		strokeColor = col;
	}
	else if (colorModeFlag == HSB)
	{
		strokeColor = colorHSB(Color{ col.r, col.g, col.b });
	}
	else if (colorModeFlag == HSL)
	{
		strokeColor = colorHSL(Color{ col.r, col.g, col.b });
	}
}

void stroke(uint8 col)
{
	if (colorModeFlag == RGB)
	{
		strokeColor.r = col;
		strokeColor.g = col;
		strokeColor.b = col;
	}
	else if (colorModeFlag == HSB)
	{
		strokeColor = colorHSB(Color{ col, col, col });
	}
	else if (colorModeFlag == HSL)
	{
		strokeColor = colorHSL(Color{ col, col, col });
	}
}

int32 color32(int32 r, int32 g, int32 b)
{
	/*if (r > 255)
	r = 255;
	if (g > 255)
	g = 255;
	if (b > 255)
	b = 255;*/
	/*if (r < 0)
	r = 0;
	if (g < 0)
	g = 0;
	if (b < 0)
	b = 0;*/
	int32 color = r << 16 | g << 8 | b;
	return color;
}

//set lineWidth on shapes and lines
void strokeWeight(int32 value)
{
	lineWidth = value;
}

void noStroke()
{
	lineWidth = 0;
}

/* clear the pixelbuffer with specific color */
/*inline void clear(int32 col)
{
	//memset(pixelBuffer, color, screenWidth * screenHeight * 4);
	void *memory = pixelBuffer;
	int32 count = screenWidth * screenHeight;

	/* memory fill */
	/*_asm
	{
		cld								//clear direction flag
		mov edi, memory					//move pixelBuffer pointer into edi
		mov ecx, count					//move loop counter into ecx
		mov eax, col					//move color into eax
		rep stosd						//fill memory
	}
}*/

inline void clear(uint8 inCol)
{
	uint32 col;
	Color c;
	switch (colorModeFlag)
	{
	case HSB:
		c = colorHSB(Color{ (int32)inCol, (int32)inCol, (int32)inCol });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	case HSL:
		c = colorHSL(Color{ (int32)inCol, (int32)inCol, (int32)inCol });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	default:	//RGB
		col = inCol << 16 | inCol << 8 | inCol | 0xff000000;
		break;
	}

	memset(pixelBuffer, col, screenWidth * screenHeight * 4);
}

inline void clear(uint8 r, uint8 g, uint8 b)
{
	uint32 col;
	Color c;
	switch (colorModeFlag)
	{
	case HSB:
		c = colorHSB(Color{ r, g, b });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	case HSL:
		c = colorHSL(Color{ r, g, b });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	default:	//RGB
		col = r << 16 | g << 8 | b | 0xff000000;
		break;
	}
	//memset(pixelBuffer, col, screenWidth * screenHeight * 4);
	void *memory = pixelBuffer;
	int32 count = screenWidth * screenHeight;

	_asm
	{
		cld
		mov edi, memory
		mov ecx, count
		mov eax, col
		rep stosd
	}
}

inline void clear(Color inCol)
{
	uint32 col;
	Color c;
	switch (colorModeFlag)
	{
	case HSB:
		c = colorHSB(Color{ inCol.r, inCol.g, inCol.b });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	case HSL:
		c = colorHSL(Color{ inCol.r, inCol.g, inCol.b });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	default:	//RGB
		col = inCol.r << 16 | inCol.g << 8 | inCol.b | 0xff000000;
		break;
	}
	//memset(pixelBuffer, col, screenWidth * screenHeight * 4);

	void *memory = pixelBuffer;
	int32 count = screenWidth * screenHeight;

	_asm
	{
		cld
		mov edi, memory
		mov ecx, count
		mov eax, col
		rep stosd
	}
}

inline void pixel(int32 x, int32 y)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	int32 col = strokeColor.r << 16 | strokeColor.g << 8 | strokeColor.b | 0xff000000;
	pixelBuffer[y*screenWidth + x] = col;
}

inline void pixel(int32 x, int32 y, uint32 col)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	pixelBuffer[y*screenWidth + x] = col;
}

inline void pixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	int32 color = r << 16 | g << 8 | b | 0xff000000;
	pixelBuffer[y*screenWidth + x] = color;
}

inline void pixel(int32 x, int32 y, Color col)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	int32 c = col.r << 16 | col.g << 8 | col.b | 0xff000000;
	pixelBuffer[y*screenWidth + x] = c;
}

inline void hzLine(int y, int x1, int x2, Color c)
{
	if (x2 < x1) { x1 += x2; x2 = x1 - x2; x1 -= x2; } //swap x1 and x2
	if (x2 < 0 || x1 >= screenWidth || y < 0 || y >= screenHeight) return;
	if (x1 < 0) x1 = 0; //clip
	if (x2 >= screenWidth) x2 = screenWidth - 1; //clip

	int col = c.r << 16 | c.g << 8 | c.b | 0xff000000;

	//int thickness{ 0 };

	//for (int i = 0; i < lineWidth; i++)
	//{
	for (int x = x1; x <= x2; x++)
	{
		pixelBuffer[(y/* + thickness*/)*screenWidth + x] = col;
	}
	//thickness++;
	//}
}

inline void vtLine(int x, int y1, int y2, Color c)
{
	if (y2 < y1) { y1 += y2; y2 = y1 - y2; y1 -= y2; } //swap y1 and y2
	if (y2 < 0 || y1 >= screenHeight || x < 0 || x >= screenWidth) return;
	if (y1 < 0) y1 = 0; //clip
	if (y2 >= screenWidth) y2 = screenHeight - 1; //clip

	int col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
	for (int y = y1; y <= y2; y++)
	{
		pixelBuffer[y*screenWidth + x] = col;
	}
}

//draws a line with Breshenam's algorithm
inline void line(int32 x1, int32 y1, int32 x2, int32 y2)
{

		bool32 step = abs(x2 - x1) < abs(y2 - y1);

		//rotate the line
		if (step)
		{
			SWAP(x1, y1);
			SWAP(x2, y2);
		}

		if (x2 < x1)
		{
			SWAP(x1, x2);
			SWAP(y1, y2);
		}

		//Bresenham’s algorithm starts by plotting a pixel at the first coordinate of the line
		//(x1, y1), and to x+1, it takes the difference of the y component of the line to the
		//two possible y coordinates, and uses the y coordinate where the error is the smaller,
		//and repeats this for every pixel.
		int32 templineWidth = 0;
		//TODO: fix lineWidth
		//for (int i = 0; i < lineWidth; i++)
		//{
			float32 error = 0.0;

			//line slope
			float32 slope = (float32)abs(y2 - y1) / (x2 - x1);

			//starting point
			int32 y = y1;

			int32 ystep = (y2 > y1 ? 1 : -1);

			for (int32 i = x1; i < x2; i++)
			{
				if (step)
					pixel(y, i);
				else
					pixel(i, y);

				error += slope;

				if (error >= 0.5)
				{
					y += ystep;
					error -= 1.0;
				}
			}
			//templineWidth++;
		//}
}

inline void rect(int x, int y, int width, int height)
{
	if (fillFlag)
	{
		//save values
		int tempX = x;
		int tempY = y;
		int tempW = width;
		int tempH = height;

		//to prevent that the lineWidth is bigger than the square
		if (lineWidth >= 1 && lineWidth < (width -lineWidth) && lineWidth < (height - lineWidth))
		{
			//draw black lineWidth
			while (height--)
			{
				//draw horizontal line
				while (width--)
				{
					pixel(x, y, strokeColor);
					x++;
				}

				//next row
				width = tempW;
				x = tempX;
				y++;
			}

			x = tempX;
			y = tempY;
			width = tempW;
			height = tempH;
			x += lineWidth;
			y += lineWidth;
			width -= lineWidth*2;
			height -= lineWidth*2;

			tempX = x;
			tempW = width;

			while (height--)
			{
				//draw horizontal line
				while (width--)
				{
					pixel(x, y, fillColor);
					x++;
				}

				//next row
				width = tempW;
				x = tempX;
				y++;
			}
		}
		else
		{
			while (height--)
			{
				//draw horizontal line
				while (width--)
				{
					pixel(x, y);
					x++;
				}

				//next row
				width = tempW;
				x = tempX;
				y++;
			}
		}
		
	}
	else
	{
		//draw non filled rect
		line(x, y, x + width, y);
		line(x, y, x, y + height);
		line(x, y + height, x + width, y + height);
		line(x + width, y, x + width, y + height);
	}

}

//Bresenham circle algorithm
inline void circle2(int32 x, int32 y, int32 radius)
{
	if (fillFlag)
	{
		//NOTE: fix-> extremly slow with windows GDI, why?
		int32 r2 = radius * radius;
		int32 area = r2 << 2;
		int32 rr = radius << 1;
		if (lineWidth >= 1)
		{
			for (int32 i = 0; i < area; i++)
			{
				int32 tx = (i % rr) - radius;
				int32 ty = (i / rr) - radius;

				if (tx * tx + ty * ty <= r2)
					pixel(x + tx, y + ty);
			}
			radius -= lineWidth;
			r2 = radius * radius;
			area = r2 << 2;
			rr = radius << 1;
			//x++;
			//y++;
			for (int32 i = 0; i < area; i++)
			{
				int32 tx = (i % rr) - radius;
				int32 ty = (i / rr) - radius;

				if (tx * tx + ty * ty <= r2)
					pixel(x + tx, y + ty, fillColor);
			}
		}
		else
		{
			for (int32 i = 0; i < area; i++)
			{
				int32 tx = (i % rr) - radius;
				int32 ty = (i / rr) - radius;

				if (tx * tx + ty * ty <= r2)
					pixel(x + tx, y + ty, fillColor);
			}
		}

	}
	else
	{
		int32 x1 = radius;
		int32 y1 = 0;
		int32 err = 0;

		while (x1 >= y1)
		{
			pixel(x + x1, y + y1);
			pixel(x + y1, y + x1);
			pixel(x - y1, y + x1);
			pixel(x - x1, y + y1);
			pixel(x - x1, y - y1);
			pixel(x - y1, y - x1);
			pixel(x + y1, y - x1);
			pixel(x + x1, y - y1);

			y1 += 1;
			err += 1 + 2 * y1;
			if (2 * (err - x1) + 1 > 0)
			{
				x1 -= 1;
				err += 1 - 2 * x1;
			}
		}
	}
}

inline void fillCircleData(int xc, int yc, int p, int pb, int pd, int radius, Color col)
{
	int a, b, c, d, e, f, g, h;
	int x = 0;
	int y = radius;
	while (x <= y)
	{
		a = xc + x;
		b = yc + y;
		c = xc - x;
		d = yc - y;
		e = xc + y;
		f = yc + x;
		g = xc - y;
		h = yc - x;
		if (b != pb) hzLine(b, a, c, col);
		if (d != pd) hzLine(d, a, c, col);
		if (f != b)  hzLine(f, e, g, col);
		if (h != d && h != f) hzLine(h, e, g, col);
		pb = b;
		pd = d;
		if (p < 0) p += (x++ << 2) + 6;
		else p += ((x++ - y--) << 2) + 10;
	}
}

inline void circle(int xc, int yc, int radius)
{
	//radius += lineWidth;
	if (xc + radius < 0 || xc - radius >= screenWidth || yc + radius < 0 || yc - radius >= screenHeight) return;

	if (fillFlag)
	{
		
		int p = 3 - (radius << 1);
		int pb = yc + radius + 1, pd = yc + radius + 1; //previous values: to avoid drawing horizontal lines multiple times  (ensure initial value is outside the range)
		if (lineWidth >= 1)
		{
			fillCircleData(xc, yc, p, pb, pd, radius, strokeColor);

			radius -= lineWidth;
			
			int p = 3 - (radius << 1);
			int pb = yc + radius + 1, pd = yc + radius + 1; 
			fillCircleData(xc, yc, p, pb, pd, radius, fillColor);
		}
		else
		{
			fillCircleData(xc, yc, p, pb, pd, radius, fillColor);
		}
	}
	else
	{
		int32 x1 = radius;
		int32 y1 = 0;
		int32 err = 0;

		while (x1 >= y1)
		{
			pixel(xc + x1, yc + y1);
			pixel(xc + y1, yc + x1);
			pixel(xc - y1, yc + x1);
			pixel(xc - x1, yc + y1);
			pixel(xc - x1, yc - y1);
			pixel(xc - y1, yc - x1);
			pixel(xc + y1, yc - x1);
			pixel(xc + x1, yc - y1);

			y1 += 1;
			err += 1 + 2 * y1;
			if (2 * (err - x1) + 1 > 0)
			{
				x1 -= 1;
				err += 1 - 2 * x1;
			}
		}
	}
}

int32 xOffset = 400;
int32 yOffset = 200;

void draw2dPolygon(vec2 *inVertices, int32 numVertices)
{

	for (int i = 0; i < numVertices; i++)
	{
	
		line((int32)(inVertices[i].x + xOffset),						
			(int32)(-inVertices[i].y + yOffset),						
			(int32)(inVertices[(i + 1) % numVertices].x + xOffset),		
			(int32)(-inVertices[(i + 1) % numVertices].y + yOffset));	
	}
}


void translate2d(vec2 *inVertices, vec2 *outVertices, int32 numVertices, float32 x, float32 y)
{
	for (int32 i = 0; i < numVertices; i++)
	{
		outVertices[i].x = inVertices[i].x + x;
		outVertices[i].y = inVertices[i].y + y;
	}
}

void scale2d(vec2 *inVertices, vec2 *outVertices, int32 Total, float32 scale)
{
	for (int32 i = 0; i < Total; i++)
	{
		outVertices[i].x = inVertices[i].x * scale;
		outVertices[i].y = inVertices[i].y * scale;
	}
}

void rotateZ2d(vec2 *inVertices, vec2 *outVertices, int32 numVertices, float32 angle)
{
	for (int32 i = 0; i < numVertices; i++)
	{
		outVertices[i].x = inVertices[i].x * cos(angle) - inVertices[i].y * sin(angle);
		outVertices[i].y = inVertices[i].y * cos(angle) + inVertices[i].x * sin(angle);
	}
}

void triangle(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3)
{
	vec2 triangleVertices[3] =
	{
		{ x1, y1 },		// top right
		{ x2, y2 },		// top left
		{ x3, y3 },		// bottom left
	};

	vec2 SVertices[3];
	vec2 SRVertices[3];	//Scaled and Rotated 
	vec2 SRTPoints[4];	//Scaled, Rotated, Translated

	//prevents scaling with negative number
	//if (scale < 0) scale = 0;				

	//transformations
	scale2d(triangleVertices, SVertices, 3, 1);
	rotateZ2d(SVertices, SRVertices, 3, 90);
	translate2d(SRVertices, SRTPoints, 3, 100, 100);

	draw2dPolygon(SRTPoints, 3);
}

#endif
