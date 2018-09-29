	uint8 sprite0[16 * 16] =
	{
		0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0
	};

void clear(Color inCol)
{
	uint32 col;
	Color c;
	switch (state.colorModeFlag)
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
	uint32 *pixel = (uint32 *)pixels;

	for (int32 y = 0; y < windowWidth; y++)
	{
		for (int32 x = 0; x < windowHeight; x++)
		{
			*pixel++ = col;
		}
	}

	//x86 build only
	/*void *memory = pixels;
	int32 count = windowWidth * windowHeight;
	_asm
	{
	cld
	mov edi, memory
	mov ecx, count
	mov eax, col
	rep stosd
	}*/
}

void clear(int32 col)
{
	Color tempCol;
	int32 r = (col & 0xff0000) >> 16;
	int32 g = (col & 0x00ff00) >> 8;
	int32 b = col & 0x0000ff;
	tempCol = {r, g, b, 255 };

	clear(tempCol);
}

void clear(uint8 r, uint8 g, uint8 b)
{
	Color tempCol = { r, g, b, 255 };
	clear(tempCol);
}

void clear(uint8 inCol)
{
	uint32 col;
	Color c;
	switch (state.colorModeFlag)
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

	uint32 *pixel = (uint32 *)pixels;

	for (int32 y = 0; y < windowWidth; y++)
	{
		for (int32 x = 0; x < windowHeight; x++)
		{
			*pixel++ = col;
		}
	}
}


//set lineWidth on shapes and lines
void strokeWeight(int32 value)
{
	state.lineWidth = value;
}

void noStroke()
{
	state.lineWidth = 0;
}

void stroke(Color col)
{
	Color tempCol;
	
	if (state.lineWidth == 0)
		state.lineWidth = 1;

	if (state.colorModeFlag == HSB)
		tempCol = colorHSB(Color{ col.r, col.g, col.b });
	else if (state.colorModeFlag == HSL)
		tempCol = colorHSL(Color{ col.r, col.g, col.b });
	else //RGB
		tempCol = col;

	strokeColor.r = tempCol.r;
	strokeColor.g = tempCol.g;
	strokeColor.b = tempCol.b;
	strokeColor.a = tempCol.a;
}

void stroke(Color col, int32 alpha)
{
	Color tempCol;
	
	if (state.lineWidth == 0)
		state.lineWidth = 1;

	if (state.colorModeFlag == HSB)
		tempCol = colorHSB(Color{ col.r, col.g, col.b });
	else if (state.colorModeFlag == HSL)
		tempCol = colorHSL(Color{ col.r, col.g, col.b });
	else //RGB
		tempCol = col;

	strokeColor.r = tempCol.r;
	strokeColor.g = tempCol.g;
	strokeColor.b = tempCol.b;
	strokeColor.a = alpha;
}

void stroke(int32 col)
{
	if (state.lineWidth == 0)
		state.lineWidth = 1;

	int32 r = (col & 0xff0000) >> 16;
	int32 g = (col & 0x00ff00) >> 8;
	int32 b = col & 0x0000ff;

	strokeColor.r = r;
	strokeColor.g = g;
	strokeColor.b = b;
	strokeColor.a = 255;
}

void stroke(int32 r, int32 g, int32 b, int32 alpha = 255)
{
	if (state.lineWidth == 0)
		state.lineWidth = 1;

	Color tempCol;
	tempCol.r = r;
	tempCol.g = g;
	tempCol.b = b;
	tempCol.a = alpha;
	stroke(tempCol);
}

void stroke(uint8 col, int32 alpha = 255)
{
	if (state.lineWidth == 0)
		state.lineWidth = 1;

	Color tempCol;
	
	if (state.colorModeFlag == HSB)
		tempCol = colorHSB(Color{ col, col, col });
	else if (state.colorModeFlag == HSL)
		tempCol = colorHSL(Color{ col, col, col });
	else //RGB
		tempCol = { col, col, col, alpha };

	strokeColor.r = tempCol.r;
	strokeColor.g = tempCol.g;
	strokeColor.b = tempCol.b;
	strokeColor.a = alpha;
}


void fill(Color col)
{
	state.fillFlag = true;

	Color tempCol;
	if (state.colorModeFlag == HSB)
		tempCol = colorHSB(Color{ col.r, col.g, col.b });
	else if (state.colorModeFlag == HSL)
		tempCol = colorHSL(Color{ col.r, col.g, col.b });
	else //RGB
		tempCol = col;

	fillColor.r = tempCol.r;
	fillColor.g = tempCol.g;
	fillColor.b = tempCol.b;
	fillColor.a = col.a;
}

void fill(Color col, uint8 alpha)
{
	state.fillFlag = true;

	Color tempCol;
	if (state.colorModeFlag == HSB)
		tempCol = colorHSB(Color{ col.r, col.g, col.b });
	else if (state.colorModeFlag == HSL)
		tempCol = colorHSL(Color{ col.r, col.g, col.b });
	else //RGB
		tempCol = col;

	fillColor.r = tempCol.r;
	fillColor.g = tempCol.g;
	fillColor.b = tempCol.b;
	fillColor.a = alpha;
}

/*
void fill(int32 col)
{
	Color tempCol;
	int32 r = (col & 0xff0000) >> 16;
	int32 g = (col & 0x00ff00) >> 8;
	int32 b = col & 0x0000ff;
	tempCol = {r, g, b, 255 };

	fill(tempCol);
}
*/
void fill(int32 r, int32 g, int32 b, int32 alpha = 255)
{
	state.fillFlag = true;
	Color tempCol = { r, g, b, alpha };
	fill(tempCol);
}

void fill(uint8 col, int32 alpha = 255)
{
	state.fillFlag = true;
	Color tempCol;
	
	if (state.colorModeFlag == HSB)
		tempCol = colorHSB(Color{ col, col, col });
	else if (state.colorModeFlag == HSL)
		tempCol = colorHSL(Color{ col, col, col });
	else //RGB
		tempCol = { col, col, col, alpha };

	fillColor.r = tempCol.r;
	fillColor.g = tempCol.g;
	fillColor.b = tempCol.b;
	fillColor.a = alpha;
}

int32 color32(uint8 r, uint8 g, uint8 b)
{
	/*if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;
	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;*/
	int32 col = r << 16 | g << 8 | b;
	return col;
}

int32 color32(Color inColor)
{
	/*if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;
	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;*/
	int32 col = inColor.r << 16 | inColor.g << 8 | inColor.b;
	return col;
}

//blending
void plotBlendedPixel(int32 x, int32 y, Color col)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;
	real32 a = col.a / 255.0f; //to get a number between 0.0-1.0
	real32 sR = (real32)col.r;
	real32 sG = (real32)col.g;
	real32 sB = (real32)col.b;

	uint32 tempcol = pixels[y*windowWidth + x];

	real32 dR = (real32)((tempcol >> 16) & 0xff);
	real32 dB = (real32)((tempcol >> 8) & 0xff);
	real32 dG = (real32)((tempcol >> 8) & 0xff);


	//linear blend
	real32 r = (1.0f - a)*dR + a*sR;
	real32 g = (1.0f - a)*dG + a*sG;
	real32 b = (1.0f - a)*dB + a*sB;

	pixels[y*windowWidth + x] = (((uint32)(r + 0.5f) << 16) |
		((uint32)(g + 0.5f) << 8) |
		((uint32)(b + 0.5f) << 0));
}

void drawFilledRect(int32 startX, int32 startY, Color col, int32 width, int32 height);

static void point(int32 x, int32 y, int32 width)
{
	drawFilledRect(x, y, strokeColor, width, width);
}

inline void pixel(int32 x, int32 y)
{
	if ((x < 0) || (x > windowWidth - 1) || (y < 0) || (y > windowHeight - 1)) return;

	if (state.lineWidth > 1)
	{
		int32 tempWidth = state.lineWidth;
		state.lineWidth = 0;
		point(x, y, tempWidth);
		state.lineWidth = tempWidth;
	}
	//alpha blending on
	else if (strokeColor.a < 255)
		plotBlendedPixel(x, y, strokeColor);
	else
	{
		int32 col = strokeColor.r << 16 | strokeColor.g << 8 | strokeColor.b | 0xff000000;
		pixels[y*windowWidth + x] = col;

	}
}
inline void pixel(int32 x, int32 y, uint32 col)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;
	pixels[y*windowWidth + x] = col;
}

inline void pixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;
	int32 col = r << 16 | g << 8 | b | 0xff000000;
	pixels[y*windowWidth + x] = col;
}

inline void pixel(int32 x, int32 y, Color col)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;

	if (state.lineWidth > 1)
	{
		int32 tempWidth = state.lineWidth;
		state.lineWidth = 0;
		point(x, y, tempWidth);
		state.lineWidth = tempWidth;
	}
	//alpha blending on
	else if (col.a < 255)
		plotBlendedPixel(x, y, col);
	else
	{
		int32 c = col.r << 16 | col.g << 8 | col.b | 0xff000000;
		pixels[y*windowWidth + x] = c;
	}
}

uint32 getPixel(int32 x, int32 y)
{
	uint32 col = pixels[y*windowWidth + x];
	return col;
}

void hzLine(int y, int x1, int x2, Color c)
{
	if (x2 < x1) { x1 += x2; x2 = x1 - x2; x1 -= x2; } //swap x1 and x2
	if (x2 < 0 || x1 >= windowWidth || y < 0 || y >= windowHeight) return;
	if (x1 < 0) x1 = 0; //clip
	if (x2 >= windowWidth) x2 = windowWidth - 1; //clip

	for (int x = x1; x <= x2; x++)
	{
		//pixels[(y/* + thickness*/)*windowWidth + x] = col;
		pixel(x, y, c);
	}
}

void vtLine(int x, int y1, int y2, Color c)
{
	if (y2 < y1) { y1 += y2; y2 = y1 - y2; y1 -= y2; } //swap y1 and y2
	if (y2 < 0 || y1 >= windowHeight || x < 0 || x >= windowWidth) return;
	if (y1 < 0) y1 = 0; //clip
	if (y2 >= windowWidth) y2 = windowHeight - 1; //clip

	int col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
	for (int y = y1; y <= y2; y++)
	{
		pixels[y*windowWidth + x] = col;
		//pixel(x, y, c);
	}
}

void drawFilledRect(int32 startX, int32 startY, Color col, int32 width, int32 height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			pixel(startX + x, startY + y, col);
			//plotBlendedPixel(startX + x, startY + y, col);
		}

	}
}

//draws a line with Breshenam's algorithm
inline void line(int32 x1, int32 y1, int32 x2, int32 y2)
{
	if (state.lineWidth <= 0)
		return;

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

	//Bresenham's algorithm starts by plotting a pixel at the first coordinate of the line
	//(x1, y1), and to x+1, it takes the difference of the y component of the line to the
	//two possible y coordinates, and uses the y coordinate where the error is the smaller,
	//and repeats this for every pixel.
	bool32 drawRect = false;

	real32 error = 0.0;

	//line slope
	real32 slope = (real32)abs(y2 - y1) / (x2 - x1);

	//starting point
	int32 y = y1;

	int32 ystep = (y2 > y1 ? 1 : -1);

	if (state.lineWidth > 1)
	{
		drawRect = true;
		//ystep = (y2 > y1 ? lineWidth : -lineWidth);
	}
	//int32 xStep = lineWidth;
	for (int32 i = x1; i < x2; i += 1)
	{
		if (step)
		{
			if (drawRect)
				drawFilledRect(y, i, strokeColor, state.lineWidth, state.lineWidth);
			else
				pixel(y, i, strokeColor);

		}
		else
		{
			if (drawRect)
			{
				drawFilledRect(i, y, strokeColor, state.lineWidth, state.lineWidth);
			}
			else
				pixel(i, y, strokeColor);
		}
		error += slope;

		if (error >= 0.5)
		{
			y += ystep;
			error -= 1.0;
			//	i += 1;
		}
		//else
		//	i += 1;

	}
}

inline void line(real32 x1, real32 y1, real32 x2, real32 y2)
{
	line((int32)x1, (int32)y1, (int32)x2, (int32)y2);
}

void rect(int32 x, int32 y, int32 width, int32 height)
{
	//to prevent that the lineWidth is bigger than the square
	if (state.lineWidth >= 1 && state.lineWidth < (width - state.lineWidth) && state.lineWidth < (height - state.lineWidth))
	{
		//draw non filled rect
		line(x, y, x + width, y);						//top
		line(x, y, x, y + height);			//left
		line(x, y + height, x + width, y + height); //bottom
		line(x + width, y, x + width, y + height);		//right
														//change the values so we only fill inside the rect
		x += state.lineWidth;
		y += state.lineWidth;
		width -= state.lineWidth;
		height -= state.lineWidth;
	}

	if (state.fillFlag)
	{
		drawFilledRect(x, y, fillColor, width, height);
	}
}

void rect(int32 x, int32 y, int32 width, int32 height, int32 col)
{
	int32 r = (col & 0xff0000) >> 16;
	int32 g = (col & 0x00ff00) >> 8;
	int32 b = col & 0x0000ff;
	Color tempCol = {fillColor.r, fillColor.g, fillColor.b, 255};
	fill(Color{ r,g,b,255 });
	rect(x, y, width, height);
	fillColor = tempCol;
}

inline void rect(real32 x, real32 y, real32 width, real32 height)
{
	rect((int32)x, (int32)y, (int32)width, (int32)height);
}

inline void rect(real32 x, real32 y, real32 width, real32 height, int32 col)
{
	rect((int32)x, (int32)y, (int32)width, (int32)height, col);
}

//Bresenham circle algorithm
void circle2(int32 x, int32 y, int32 radius)
{
	if (state.fillFlag)
	{
		//NOTE: fix-> extremly slow with windows GDI, why?
		int32 r2 = radius * radius;
		int32 area = r2 << 2;
		int32 rr = radius << 1;
		if (state.lineWidth >= 1)
		{
			for (int32 i = 0; i < area; i++)
			{
				int32 tx = (i % rr) - radius;
				int32 ty = (i / rr) - radius;

				if (tx * tx + ty * ty <= r2)
					pixel(x + tx, y + ty);
			}
			radius -= state.lineWidth;
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

static void fillCircleData(int xc, int yc, int p, int pb, int pd, int radius, Color col)
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

void circle(int32 xc, int32 yc, int32 radius)
{
	//radius += lineWidth;
	if (xc + radius < 0 || xc - radius >= windowWidth || yc + radius < 0 || yc - radius >= windowHeight) return;

	if (state.fillFlag)
	{

		int p = 3 - (radius << 1);
		int pb = yc + radius + 1, pd = yc + radius + 1; //previous values: to avoid drawing horizontal lines multiple times  (ensure initial value is outside the range)
		if (state.lineWidth >= 1)
		{
			//fillCircleData(xc, yc, p, pb, pd, radius, strokeColor);

			radius -= state.lineWidth;

			p = 3 - (radius << 1);
			pb = yc + radius + 1, pd = yc + radius + 1;
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

void circle(real32 inX, real32 inY, real32 radius)
{
	circle((int32)inX, (int32)inY, (int32)radius);
}

void draw2dPolygon(vec2 *inVertices, int32 numVertices)
{
	for (int i = 0; i < numVertices; i++)
	{
		line((int32)(inVertices[i].x + center.x),
			(int32)(-inVertices[i].y + center.y),
			(int32)(inVertices[(i + 1) % numVertices].x + center.x),
			(int32)(-inVertices[(i + 1) % numVertices].y + center.y));
	}
}


void translate2d(vec2 *inVertices, vec2 *outVertices, int32 numVertices, real32 x, real32 y)
{
	for (int32 i = 0; i < numVertices; i++)
	{
		outVertices[i].x = inVertices[i].x + x;
		outVertices[i].y = inVertices[i].y + y;
	}
}

void scale2d(vec2 *inVertices, vec2 *outVertices, int32 Total, real32 scale)
{
	for (int32 i = 0; i < Total; i++)
	{
		outVertices[i].x = inVertices[i].x * scale;
		outVertices[i].y = inVertices[i].y * scale;
	}
}

void rotateZ2d(vec2 *inVertices, vec2 *outVertices, int32 numVertices, real32 angle)
{
	for (int32 i = 0; i < numVertices; i++)
	{
		outVertices[i].x = inVertices[i].x * cos(angle) - inVertices[i].y * sin(angle);
		outVertices[i].y = inVertices[i].y * cos(angle) + inVertices[i].x * sin(angle);
	}
}

void triangle(real32 x1, real32 y1, real32 x2, real32 y2, real32 x3, real32 y3)
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


void softSprite(uint8 *spriteData, int32 x, int32 y, int32 width, int32 height, Color col)
{
	if ((x<0) || (x>windowWidth - 16) || (y<0) || (y>windowHeight - 16)) return;
	int32 c = col.r << 16 | col.g << 8 | col.b | 0xff000000;
	//uint8 *sprite = spriteMap[id];

	int32 index=0;
	int32 yOffset = y * windowWidth + x;
	for (int32 i = 0; i < height; i++)
	{
		for (int32 j = 0; j < width; j++)
		{
			if (spriteData[index++])
			{
				((uint32*)pixels)[yOffset + j] = c;
			}
		}
		yOffset += windowWidth;
	}
}