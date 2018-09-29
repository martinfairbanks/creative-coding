bool32 projection3DFlag = false;				//2D projection if false
bool32 doubleBufferDisabledFlag = false;	//turn off double buffer

enum { CENTER = 1 };
int32 rectModeFlag = 0;

void rectMode(const int32 mode)
{
	rectModeFlag = mode;
}

void fill(int32 inR, int32 inG, int32 inB, int32 inA = 255)
{
	state.fillFlag = true;
	Color newColor = checkColorMode(inR, inG, inB);

	real32 r = real32(newColor.r / 255.0);
	real32 g = real32(newColor.g / 255.0);
	real32 b = real32(newColor.b / 255.0);
	real32 a = real32(inA / 255.0);

	fillColor.r = r;
	fillColor.g = g;
	fillColor.b = b;
	fillColor.a = a;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
/*
inline void fill(real32 r, real32 g, real32 b, real32 a = 1.f)
{
state.fillFlag = true;
Color tempCol;
ColorRGBA tempRGBACol;

if (state.colorModeFlag == HSB || state.colorModeFlag == HSL)
{
int32 red = int32(tempCol.r * 255.0);
int32 green = int32(tempCol.g * 255.0);
int32 blue = int32(tempCol.b * 255.0);
int32 alpha = int32(a * 255.0);

if (state.colorModeFlag == HSB)
{
tempCol = colorHSB(Color{ red, green, blue });
}
else if (state.colorModeFlag == HSL)
{
tempCol = colorHSL(Color{ red, green, blue });
}

tempRGBACol.r = real32(tempCol.r / 255.0);
tempRGBACol.g = real32(tempCol.g / 255.0);
tempRGBACol.b = real32(tempCol.b / 255.0);
tempRGBACol.a = a;
}

if (state.colorModeFlag == RGB)
{
tempRGBACol.r = r;
tempRGBACol.g = g;
tempRGBACol.b = b;
tempRGBACol.a = a;
}

fillColor.r = tempRGBACol.r;
fillColor.g = tempRGBACol.g;
fillColor.b = tempRGBACol.b;
fillColor.a = tempRGBACol.a;

glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
*/
/*void fill(Color col, int32 alpha = 255)
{
	state.fillFlag = true;
	Color newColor = checkColorMode(col.r, col.g, col.b);
	real32 r = real32(newColor.r / 255.0);
	real32 g = real32(newColor.g / 255.0);
	real32 b = real32(newColor.b / 255.0);
	real32 a = real32(alpha / 255.0);

	fillColor.r = r;
	fillColor.g = g;
	fillColor.b = b;
	fillColor.a = a;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}*/

inline void fill(Color col)
{
	state.fillFlag = true;
	Color newColor = checkColorMode(col.r, col.g, col.b);
	real32 r = real32(newColor.r / 255.0);
	real32 g = real32(newColor.g / 255.0);
	real32 b = real32(newColor.b / 255.0);
	real32 a = real32(col.a / 255.0);

	fillColor.r = r;
	fillColor.g = g;
	fillColor.b = b;
	fillColor.a = a;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/*void fill(int32 col, int32 alpha = 255)
{
	state.fillFlag = true;
	Color newColor = checkColorMode(col, col, col);

	real32 r = real32(newColor.r / 255.0);
	real32 g = real32(newColor.g / 255.0);
	real32 b = real32(newColor.b / 255.0);
	real32 a = real32(alpha / 255.0);

	fillColor.r = r;
	fillColor.g = g;
	fillColor.b = b;
	fillColor.a = a;
}*/

inline void noFill()
{
	state.fillFlag = false;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

//clear screen buffer
//TODO: checkColorMode
void clear(real32 col)
{
	glClearColor(col, col, col, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clear(int32 col)
{
	real32 tempCol = real32(col / 255.0);
	glClearColor(tempCol, tempCol, tempCol, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clear(real32 r, real32 g, real32 b, real32 a = 1.f)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clear(int32 inR, int32 inG, int32 inB, int32 inA = 255)
{
	Color newColor = checkColorMode(inR, inG, inB);

	real32 r = real32(newColor.r / 255.0);
	real32 g = real32(newColor.g / 255.0);
	real32 b = real32(newColor.b / 255.0);
	real32 a = real32(inA / 255.0);

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clear(Color col)
{
	Color newColor = checkColorMode(col.r, col.g, col.b);

	real32 r = real32(newColor.r / 255.0);
	real32 g = real32(newColor.g / 255.0);
	real32 b = real32(newColor.b / 255.0);
	real32 a = real32(col.a / 255.0);

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
void clear(Color col)
{
	Color newColor = checkColorMode(col.r, col.g, col.b);

	real32 red = real32(newColor.r / 255.0);
	real32 green = real32(newColor.g / 255.0);
	real32 blue = real32(newColor.b / 255.0);

	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
*/

void strokeWeight(int32 value)
{
	state.lineWidth = value;
	glLineWidth((real32)value);
	glPointSize((real32)value);
}

void noStroke()
{
	state.lineWidth = 0;
	glLineWidth(1);
}

void stroke(int32 inR, int32 inG, int32 inB, int32 inA = 255)
{
	Color col = { inR, inG, inB, inA };
	Color tempCol = {};
	if (state.colorModeFlag == RGB)
	{
		tempCol = col;

	}
	else if (state.colorModeFlag == HSB)
	{
		tempCol = colorHSB(col);

	}
	else if (state.colorModeFlag == HSL)
	{
		tempCol = colorHSL(col);
	}

	real32 r = real32(tempCol.r / 255.0);
	real32 g = real32(tempCol.g / 255.0);
	real32 b = real32(tempCol.b / 255.0);
	real32 a = real32(inA / 255.0);

	strokeColor.r = r;
	strokeColor.g = g;
	strokeColor.b = b;
	strokeColor.a = a;

	glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
}

void stroke(int32 col)
{
	real32 tempCol = real32(col / 255.0);
	strokeColor.r = tempCol;
	strokeColor.g = tempCol;
	strokeColor.b = tempCol;
	strokeColor.a = 1.0f;
	glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
}

void stroke(Color col)
{
	real32 r = real32(col.r / 255.0);
	real32 g = real32(col.g / 255.0);
	real32 b = real32(col.b / 255.0);
	strokeColor.r = r;
	strokeColor.g = g;
	strokeColor.b = b;
	strokeColor.a = 1.0f;
	glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
}
/*
void stroke(real32 r, real32 g, real32 b, real32 a = 1.f)
{
Color tempCol;
ColorRGBA tempRGBACol;

if (state.colorModeFlag == HSB || state.colorModeFlag == HSL)
{
int32 red = int32(r * 255.0);
int32 green = int32(g * 255.0);
int32 blue = int32(b * 255.0);
int32 alpha = int32(a * 255.0);

if (state.colorModeFlag == HSB)
{
tempCol = colorHSB(Color{ red, green, blue });
}
else if (state.colorModeFlag == HSL)
{
tempCol = colorHSL(Color{ red, green, blue });
}

tempRGBACol.r = real32(tempCol.r / 255.0);
tempRGBACol.g = real32(tempCol.g / 255.0);
tempRGBACol.b = real32(tempCol.b / 255.0);
tempRGBACol.a = a;
}

if (state.colorModeFlag == RGB)
{
tempRGBACol.r = r;
tempRGBACol.g = g;
tempRGBACol.b = b;
tempRGBACol.a = a;
}

strokeColor.r = tempRGBACol.r;
strokeColor.g = tempRGBACol.g;
strokeColor.b = tempRGBACol.b;
strokeColor.a = tempRGBACol.a;

glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
}
*/
/*void stroke(Color col)
{
	strokeColor = col;
	glColor4f(col.r, col.g, col.b, col.a);
}
*/

inline void line(int32 x0, int32 y0, int32 x1, int32 y1)
{
	glBegin(GL_LINES);
	glVertex2i(x0, y0);
	glVertex2i(x1, y1);
	glEnd();
}

inline void point(int32 x, int32 y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

inline void point(real32 x, real32 y, real32 z)
{
	glBegin(GL_POINTS);
	glVertex3f(x, y, z);
	glEnd();

}

inline void rect(int32 x, int32 y, int32 width, int32 height)
{
	//glRectf(50.0f, 50.0f, 25.0f, 25.0f);
	int32 dx=0, dy=0;
	if (state.fillFlag)
	{
		glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
		glBegin(GL_QUADS);
		if (rectModeFlag == CENTER)
		{
			dx = x - width/2;
			dy = y - height/2;
			glVertex2i(dx, dy);
			glVertex2i(dx + width, dy);
			glVertex2i(dx + width, dy + height);
			glVertex2i(dx, dy + height);
		}
		else
		{
			glVertex2i(x, y);
			glVertex2i(x + width, y);
			glVertex2i(x + width, y + height);
			glVertex2i(x, y + height);
		}
		glEnd();
		glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	}

	if (state.lineWidth > 0)
	{
		glBegin(GL_LINE_LOOP);
		if (rectModeFlag == CENTER)
		{
			dx = x - width / 2;
			dy = y - height / 2;
			glVertex2i(dx, dy);
			glVertex2i(dx + width, dy);
			glVertex2i(dx + width, dy + height);
			glVertex2i(dx, dy + height);
		}
		else
		{
			glVertex2i(x, y);
			glVertex2i(x + width, y);
			glVertex2i(x + width, y + height);
			glVertex2i(x, y + height);
		}
		glEnd();
	}
}

void quad(int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, int32 x4, int32 y4)
{
	if (state.fillFlag)
	{
		glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
		glBegin(GL_QUADS);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
		glVertex2i(x4, y4);
		glEnd();
		glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	}
	if (state.lineWidth > 0)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
		glVertex2i(x4, y4);
		glEnd();
	}
}

void circle(int32 x, int32 y, int32 radius)
{
	if (state.fillFlag)
	{
		glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2i(x, y);
		for (real32 angle = 0.f; angle < TWO_PI; angle += 0.1f)
		{
			glVertex2f((real32)x + cos(angle) * (real32)radius, (real32)y + sin(angle) * (real32)radius);
		}
		//close the circle
		glVertex2f((real32)x + cos(TWO_PI) * (real32)radius, (real32)y + sin(TWO_PI) * (real32)radius);
		glEnd();
		glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	}

	if (state.lineWidth > 0)
	{
		glBegin(GL_LINE_STRIP);
		for (float angle = 0; angle < PI * 4; angle += (PI / 50.0f))
		{
			glVertex2f((real32)x + sin(angle) * (real32)radius, (real32)y + cos(angle) * (real32)radius);
		}
		glEnd();
	}
}

void ellipse(int32 x, int32 y, int32 r1, int32 r2 = 0)
{
	if (r2 == 0)
		r2 = r1;
	if (state.fillFlag)
	{
		glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2i(x, y);
		for (real32 angle = 0.f; angle < TWO_PI+0.02f; angle += 0.02f)
		{
			glVertex2f(real32(x + cos(angle) * r1), real32(y + sin(angle) * r2));
		}

		glEnd();
		glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	}

	if (state.lineWidth > 0)
	{
		glBegin(GL_LINE_STRIP);
		for (real32 angle = 0; angle < PI * 4; angle += (PI / 50.0f))
		{
			glVertex2f((real32)x + sin(angle) * (real32)r1, (real32)y + cos(angle) * (real32)r2);
		}
		glEnd();
	}
}

void arc(int32 x, int32 y, int32 r1, int32 r2, real32 start, real32 end)
{
	if (r2 == 0)
		r2 = r1;
	if (state.lineWidth > 0)
	{
		glBegin(GL_LINE_STRIP);
		for (real32 angle = start; angle <= end; angle += 0.02f)
		{
			glVertex2f((real32)x + cos(angle) * (real32)r1, (real32)y + sin(angle) * (real32)r2);
		}
		glEnd();
	}
}

void triangle(int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3)
{
	glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glEnd();
}

enum { CLOSE = 1 };
//TODO: 
void beginShape(int32 close = 0)
{
	if (state.fillFlag)
	{
		glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
		glBegin(GL_TRIANGLE_FAN);
		//glBegin(GL_POLYGON);
	}
	else
	{
		glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);

		if (close == CLOSE)
			glBegin(GL_LINE_LOOP);
		else
			glBegin(GL_LINE_STRIP);
	}
}

void endShape()
{
	glEnd();
}

void vertex(real32 x, real32 y, real32 z = 0.f)
{
	glVertex3f(x, y, z);
}

void pushMatrix()
{
	glPushMatrix();
}

void popMatrix()
{
	glPopMatrix();
}

void loadIdentity()
{
	glLoadIdentity();
}

void translate(real32 x, real32 y, real32 z = 0.f)
{
	glTranslatef(x, y, z);
}

void rotateX(real32 angle)
{
	glRotatef(angle, 1.0f, 0.0f, 0.0f);
}

void rotateY(real32 angle)
{
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
}

void rotateZ(real32 angle)
{
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
}

//rotation in radians
void rotate(real32 angle)
{
	real32 deg = degrees(angle);
	glRotatef(deg, 0.0f, 0.0f, 1.0f);
}

void pointSize(real32 value)
{
	glPointSize(value);
}

/* 3D shapes */
void plane(real32 width, real32 height)
{
	glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	glBegin(GL_QUADS);
	//glNormal3f(0.0, 0.0, 1.0);
	//glColor3ub((GLubyte)col.r, (GLubyte)col.g, (GLubyte)col.b);
	//glColor3f(1.0, 0.0, 0.0);
	glVertex3f(width / 2, height / 2, 0.0f);
	glVertex3f(-width / 2, height / 2, 0.0f);
	glVertex3f(-width / 2, -height / 2, 0.0f);
	glVertex3f(width / 2, -height / 2, 0.0f);
	glEnd();
}

void sphere(real32 radius, int32 slices = 24, int32 stacks = 16)
{
	glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
#if 1
	real32 drho = PI / (real32)stacks;
	real32 dtheta = 2.0f * PI / (real32)slices;
	real32 ds = 1.0f / (real32)slices;
	real32 dt = 1.0f / (real32)stacks;
	real32 t = 1.0f;
	real32 s = 0.0f;

	for (int32 i = 0; i < stacks; i++)
	{
		real32 rho = (real32)i * drho;
		real32 srho = (real32)(sin(rho));
		real32 crho = (real32)(cos(rho));
		real32 srhodrho = (real32)(sin(rho + drho));
		real32 crhodrho = (real32)(cos(rho + drho));

		glBegin(GL_TRIANGLE_STRIP);
		s = 0.0f;
		for (int32 j = 0; j <= slices; j++)
		{
			real32 theta = (j == slices) ? 0.0f : j * dtheta;
			real32 stheta = (real32)(-sin(theta));
			real32 ctheta = (real32)(cos(theta));

			real32 x = stheta * srho;
			real32 y = ctheta * srho;
			real32 z = crho;

			glTexCoord2f(s, t);
			glNormal3f(x, y, z);
			glVertex3f(x * radius, y * radius, z * radius);

			x = stheta * srhodrho;
			y = ctheta * srhodrho;
			z = crhodrho;
			glTexCoord2f(s, t - dt);
			s += ds;
			glNormal3f(x, y, z);
			glVertex3f(x * radius, y * radius, z * radius);
		}
		glEnd();

		t -= dt;
	}
#else
	GLUquadric *q = gluNewQuadric();
	gluSphere(q, radius, slices, stacks);
	gluDeleteQuadric(q);
#endif

}

void torus(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor = 61, GLint numMinor = 37)
{
	glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	vec3 normal;
	GLfloat vNormal[3];
	double majorStep = 2.0f*PI / numMajor;
	double minorStep = 2.0f*PI / numMinor;
	int i, j;

	for (i = 0; i<numMajor; ++i)
	{
		double a0 = i * majorStep;
		double a1 = a0 + majorStep;
		GLfloat x0 = (GLfloat)cos(a0);
		GLfloat y0 = (GLfloat)sin(a0);
		GLfloat x1 = (GLfloat)cos(a1);
		GLfloat y1 = (GLfloat)sin(a1);

		glBegin(GL_TRIANGLE_STRIP);
		for (j = 0; j <= numMinor; ++j)
		{
			double b = j * minorStep;
			GLfloat c = (GLfloat)cos(b);
			GLfloat r = minorRadius * c + majorRadius;
			GLfloat z = minorRadius * (GLfloat)sin(b);

			glTexCoord2f((float)(i) / (float)(numMajor), (float)(j) / (float)(numMinor));
			normal.x = x0*c;
			normal.y = y0*c;
			normal.z = z / minorRadius;
			normal.normalize();
			vNormal[0] = normal.x;
			vNormal[1] = normal.y;
			vNormal[2] = normal.z;
			glNormal3fv(vNormal);
			glVertex3f(x0*r, y0*r, z);

			glTexCoord2f((float)(i + 1) / (float)(numMajor), (float)(j) / (float)(numMinor));
			vNormal[0] = x1*c;
			vNormal[1] = y1*c;
			vNormal[2] = z / minorRadius;
			glNormal3fv(vNormal);
			glVertex3f(x1*r, y1*r, z);
		}
		glEnd();
	}
}

void cylinder(GLfloat width, GLfloat height, int32 slices = 32, int32 stacks = 7)
{
	glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	GLUquadricObj *q = gluNewQuadric();
	gluCylinder(q, width, width, height, slices, stacks);
	/*glBegin(GL_TRIANGLE_FAN);
	for (real32 angle = 0; angle < 360; angle++)
	{
	glVertex2f(sin(angle) * width, cos(angle) * width);
	}
	glEnd();
	translate(0.0f, height*2, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	for (real32 angle = 0; angle < 360; angle++)
	{
	glVertex2f(sin(angle) * width, cos(angle) * width);
	}
	glEnd();*/
	gluDeleteQuadric(q);
}

void cone(GLfloat width, GLfloat height, int32 slices = 32, int32 stacks = 7)
{
	glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	GLUquadricObj *q = gluNewQuadric();
	gluCylinder(q, width, 0, height, slices, stacks);

	glBegin(GL_TRIANGLE_FAN);
	for (real32 angle = 0; angle < 360; angle++)
	{
		glVertex2f(sin(angle) * width, cos(angle) * width);
	}
	glEnd();

	gluDeleteQuadric(q);
}

void cone2(GLfloat width, GLfloat height)
{
	//glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	GLfloat x, y, angle;	// Storage for coordinates and angles
	int iPivot = 1;			// Used to flag alternating colors
							// Begin a triangle fan
	glBegin(GL_TRIANGLE_FAN);

	// Pinnacle of cone is shared vertex for fan, moved up Z axis
	// to produce a cone instead of a circle
	glVertex3f(0.0f, 0.0f, 2.0f);

	// Loop around in a circle and specify even points along the circle
	// as the vertices of the triangle fan
	for (angle = 0.0f; angle < (2.0f*PI); angle += (PI / 8.0f))
	{
		// Calculate x and y position of the next vertex
		x = sin(angle);
		y = cos(angle);

		// Alternate color between red and green
		if ((iPivot % 2) == 0)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(0.0f, 0.0f, 1.0f);

		// Increment pivot to change color next time
		iPivot++;

		// Specify the next vertex for the triangle fan
		glVertex2f(x, y);
	}

	glEnd();

	// Begin a new triangle fan to cover the bottom
	glBegin(GL_TRIANGLE_FAN);

	// Center of fan is at the origin
	glVertex2f(0.0f, 0.0f);
	for (angle = 0.0f; angle < (2.0f*PI); angle += (PI / 8.0f))
	{
		// Calculate x and y position of the next vertex
		x = sin(angle);
		y = cos(angle);

		// Alternate color between red and green
		if ((iPivot % 2) == 0)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(0.0f, 0.0f, 1.0f);

		// Increment pivot to change color next time
		iPivot++;

		// Specify the next vertex for the triangle fan
		glVertex2f(x, y);
	}

	glEnd();
}

/*NOTE: Edited from text found in the post at http://compgroups.net/comp.graphics.api.opengl/outlining-using-glPolygonOffset

[in order to draw Polygon-offset based outline around a cube] you should store all 8 vertices in a vertex array (or buffer
object if using OpenGL v1.5). Then create two sets of indices, one for the
TRIANGLE STRIP order, and one for the outline order. Then use the same
vertex array (or buffer object) with either set of indices to draw the
primitive twice, once as a filled triangle strip and once as a line loop,
using polygon offset to push the filled triangle strip back in the depth
buffer, during the first pass.
*/
void cube(real32 size = 1.f)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	for (int32 i = 0; i < 2; i++)
	{
		if (state.lineWidth == 0)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
		}

		glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		//front face
		glVertex3f(size, size, size);
		glVertex3f(-size, size, size);
		glVertex3f(-size, -size, size);
		glVertex3f(size, -size, size);

		//left face
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-size, size, size);
		glVertex3f(-size, size, -size);
		glVertex3f(-size, -size, -size);
		glVertex3f(-size, -size, size);

		//back face
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(size, size, -size);
		glVertex3f(-size, size, -size);
		glVertex3f(-size, -size, -size);
		glVertex3f(size, -size, -size);

		//right face
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(size, size, -size);
		glVertex3f(size, size, size);
		glVertex3f(size, -size, size);
		glVertex3f(size, -size, -size);

		//top face
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(size, size, size);
		glVertex3f(-size, size, size);
		glVertex3f(-size, size, -size);
		glVertex3f(size, size, -size);

		//bottom face
		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(size, -size, size);
		glVertex3f(-size, -size, size);
		glVertex3f(-size, -size, -size);
		glVertex3f(size, -size, -size);
		glEnd();

		if (state.lineWidth == 0)
			break;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	}
}

void box(real32 width, real32 height = 0, real32 depth = 0)
{
	if (height == 0 && depth == 0)
		height = depth = width;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	for (int32 i = 0; i < 2; i++)
	{
		if (state.lineWidth == 0)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
		}
		glBegin(GL_QUADS);
		//front face
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(width, height, depth);
		glVertex3f(-width, height, depth);
		glVertex3f(-width, -height, depth);
		glVertex3f(width, -height, depth);

		//left face
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-width, height, depth);
		glVertex3f(-width, height, -depth);
		glVertex3f(-width, -height, -depth);
		glVertex3f(-width, -height, depth);

		//back face
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(width, height, -depth);
		glVertex3f(-width, height, -depth);
		glVertex3f(-width, -height, -depth);
		glVertex3f(width, -height, -depth);

		//right face
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(width, height, -depth);
		glVertex3f(width, height, depth);
		glVertex3f(width, -height, depth);
		glVertex3f(width, -height, -depth);

		//top face
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(width, height, depth);
		glVertex3f(-width, height, depth);
		glVertex3f(-width, height, -depth);
		glVertex3f(width, height, -depth);

		//bottom face
		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(width, -height, depth);
		glVertex3f(-width, -height, depth);
		glVertex3f(-width, -height, -depth);
		glVertex3f(width, -height, -depth);
		glEnd();

		if (state.lineWidth == 0)
			break;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	}
}

void pyramid(real32 width, real32 height)
{
	glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	//draw pyramid
	glBegin(GL_TRIANGLES);
	//front
	//glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, height, 0.0f);		//top
										//glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-width, -width, width);     //left
										   //glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(width, -width, width);      //right

										   //right
										   //glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, height, 0.0f);
	//glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(width, -width, width);
	//glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(width, -width, -width);

	//back
	//glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, height, 0.0f);
	//glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(width, -width, -width);
	//glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-width, -width, -width);

	//left
	//glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, height, 0.0f);
	//glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-width, -width, -width);
	//glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-width, -width, width);
	glEnd();

	//bottom of pyramid
	glBegin(GL_QUADS);
	//glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(width, -width, width);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-width, -width, width);
	//glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-width, -width, -width);
	//glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(width, -width, -width);
	glEnd();
}

/* Platonic solids */

void tetrahedron()
{
	vec3 tetrahedron[4] = {
		{ 1, 1, 1 },
		{ -1, -1, 1 },
		{ -1, 1,-1 },
		{ 1, -1, -1 }
	};

}
/* Light */

//set default light
void lights()
{

	// Light values and coordinates
	real32  ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	real32  diffuseLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat lightPos[] = { 0.f, 50.0f, 100.0f, 0.0f };

	glEnable(GL_LIGHTING);

	//setup and enable light 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	//enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	//set material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void noLights()
{
	glDisable(GL_LIGHTING);
}

void ambientLight(real32 r, real32 g, real32 b, real32 a = 1.0f)
{
	glEnable(GL_LIGHTING);

	//ambient light, directionless light, dark white light
	float ambientLight[] = { r, g, b,a };

	//set light model to use ambient light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	//enable material color tracking
	glEnable(GL_COLOR_MATERIAL);

	//front material ambient and diffuse colors track glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

//diffuse light, light from a direction
void directionalLight(real32 r, real32 g, real32 b, real32 x, real32 y, real32 z)
{
	//position/direction of light
	real32 lightPos[] = { x, y, z, 1.0 };
	real32 ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	real32 diffuseLight[] = { r, g, b, 1.0f };
	glEnable(GL_LIGHTING);

	//setup and enable light 0
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glEnable(GL_LIGHT0);

	//enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	//set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void specularLight(real32 r, real32 g, real32 b, real32 x, real32 y, real32 z)
{
	//position/direction of light
	real32 lightPos[] = { x, y, z, 1.f };
	real32 ambientLight[] = { r*0.3f, g*0.3f, b*0.3f, 1.0f };
	real32 diffuseLight[] = { r*0.7f, g*0.7f, b*0.7f, 1.f };
	real32  specularLight[] = { r, g, b, 1.0f };
	real32  specularReflectance[] = { r, g, b, 1.0f };

	glEnable(GL_LIGHTING);

	//setup and enable light 0
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glEnable(GL_LIGHT0);

	//enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	//set material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//set materials specular reflectivity and shininess
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularReflectance);
	/*	The GL_SHININESS property sets the specular exponent of the material, which specifies how small and focused the specular highlight is.
	The larger the value, the more shiny and pronounced the surface. The range of this parameter is 1�128 */
	glMateriali(GL_FRONT, GL_SHININESS, 40);
}

void spotLight(real32 r, real32 g, real32 b, real32 x, real32 y, real32 z, real32 dirX, real32 dirY, real32 dirZ, real32 angle)
{

	real32 lightPos[] = { x, y, z, 1.0f };
	real32 specular[] = { r, g, b, 1.0f };
	real32 specularRef[] = { r, g, b, 1.0f };
	real32 ambient[] = { r*0.5f, g*0.5f, b*0.5f, 1.0f };
	real32 spotDir[] = { dirX, dirY, dirZ };

	glEnable(GL_LIGHTING);

	//setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);

	//set cut off angle to 60 degrees to create spot effect
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, angle);

	glEnable(GL_LIGHT0);

	//enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	//set material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//set materials specular reflectivity and shininess
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularRef);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
}


/* Blending */
enum { ALPHA_BLEND, ADDITIVE_BLEND, MULTIPLIED_BLEND };

void blendMode(int32 mode)
{
	switch (mode)
	{
	case ALPHA_BLEND:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case ADDITIVE_BLEND:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	case MULTIPLIED_BLEND:
		glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		break;
	}
}


/* Fog */
void enableFog(real32 start, real32 end, real32 r, real32 g, real32 b, real32 a)
{
	//fog setup
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, /*GL_EXP2*/GL_LINEAR);		//fog equation
													//glFogf(GL_FOG_DENSITY, 0.6f); //funkar bara om GL_FOG_MODE == GL_EXP eller GL_EXP2
													//Note that GL_FOG_START and GL_FOG_END only have an effect on GL_LINEAR fog
													//where the fog starts and ends
	glFogf(GL_FOG_START, start);
	glFogf(GL_FOG_END, end);
	real32 col[] = { r, g, b, a };
	glFogfv(GL_FOG_COLOR, col);
}

void disableFog()
{
	glDisable(GL_FOG);
}

/* Projections */
//2D projection, used for 2D drawing
void set2dProjection(int32 width = windowWidth, int32 height = windowHeight)
{
	//glViewport(0, 0, windowWidth, windowHeight);
	
	windowWidth = width;
	windowHeight = height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, windowWidth, windowHeight, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	projection3DFlag = false;
}

//set orthographic projection
//objects with the same dimension appear the same size, regardless of 
//whether they are near or far from the camera */
void ortho(real32 left = 0.f, real32 right = windowWidth, real32 bottom = windowHeight, real32 top = 0.f, real32 nearZ = 0.0f, real32 farZ = 500.f)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(left, right, bottom, top, nearZ, farZ);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	projection3DFlag = false;
}

//set perspective projection, objects further away appear smaller
void set3dProjection(int32 width = windowWidth, int32 height = windowHeight, real32 fov = 60.f, real32 nearZ = 1.0f, real32 farZ = 500.0f)
{
	//prevent divide by zero
	if (!height) return;

	windowWidth = width;
	windowHeight = height;

	real32 aspect = (GLfloat)windowWidth / (GLfloat)windowHeight;

	//set viewport to window dimensions
	glViewport(0, 0, windowWidth, windowHeight);
	//center
	//glViewport(windowWidth / 4.f, windowHeight / 4.f, windowWidth / 2.f, windowHeight / 2.f);
	
	//switch to the projection matrix and reset it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//set camera perspective
	gluPerspective(fov,		//camera angle, field of view in degrees, set to 45 degrees viewing angle
		aspect,				//aspect ratio
		nearZ,				//near z clipping coordinate
		farZ);				//far z clipping coordinate, start and end point for how deep we can draw into the screen

	//switch to GL_MODELVIEW, tells OGL that all future transformations will affect what we draw
	//reset the modelview matrix, wich is where the object information is stored, sets x,y,z to zero
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//enable depth buffer
	glEnable(GL_DEPTH_TEST);
	//don't calculate inside of objects
	//glEnable(GL_CULL_FACE);		
	//counter clock-wise polygons face out
	glFrontFace(GL_CCW);

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	projection3DFlag = true;
}

//set perspective projection, objects further away appear smaller
void perspective(real32 fov, real32 aspect,  real32 nearZ, real32 farZ)
{
	//switch to the projection matrix and reset it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//set camera perspective
	gluPerspective(fov,		//camera angle, field of view in degrees, set to 45 degrees viewing angle
		aspect,				//aspect ratio
		nearZ,				//near z clipping coordinate
		farZ);				//far z clipping coordinate, start and end point for how deep we can draw into the screen

							//switch to GL_MODELVIEW, tells OGL that all future transformations will affect what we draw
							//reset the modelview matrix, wich is where the object information is stored, sets x,y,z to zero
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//enable depth buffer
	//glEnable(GL_DEPTH_TEST);
	//don't calculate inside of objects
	//glEnable(GL_CULL_FACE);		
	//counter clock-wise polygons face out
	//glFrontFace(GL_CCW);
	projection3DFlag = true;
}

//eyex, eyey, eyez - loacation of camera  
//centerx, centery, centerz - line of sight, a vector pointing in the forward direction of the camera
//upx, upy, upz - tells which direction is the up direction
void camera(real64 eyex, real64 eyey, real64 eyez,
	real64 centerx, real64 centery, real64 centerz,
	real64 upx, real64 upy, real64 upz)
{
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}


#include <vector>

//needed for the normal vectors
struct Face
{
	int facenum;	//id for the faces from the obj file
	bool quad;		//is it a quad?
	int faces[4];

	//triangle
	Face(int facen, int f1, int f2, int f3) : facenum(facen)
	{
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
		quad = false;
	}
	//quad
	Face(int facen, int f1, int f2, int f3, int f4) : facenum(facen)
	{
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
		faces[3] = f4;
		quad = true;
	}
};


char* moveToNextLine(char *cont)
{
	while (*cont != '\n')
	{
		cont++;
	}
	//move past \n
	cont++;
	return cont;
}

int loadModel(char *filename)
{
	std::vector<vec3*> vertex;
	std::vector<Face*> faces;
	std::vector<vec3*> normals;

	char *contents = loadTextFile(filename);
	char *contentsAddress = contents;

	while (*contents != '\0')
	{
		if (*contents == 'v' && *(1 + contents) == ' ')
		{
			float tmpx, tmpy, tmpz;
			sscanf(contents, "v %f %f %f", &tmpx, &tmpy, &tmpz);
			vertex.push_back(new vec3{tmpx, tmpy, tmpz});
		}

		//parse the normals
		else if (*contents == 'v' && *(1 + contents) == 'n')
		{
			float tmpx, tmpy, tmpz;
			sscanf(contents, "vn %f %f %f", &tmpx, &tmpy, &tmpz);
			normals.push_back(new vec3{ tmpx, tmpy, tmpz });
		}

		//parse the faces, wich points that makes a face
		else if (*contents == 'f')
		{
			int id;
			int a, b, c, d;
			int spaceCount = 0;

			//check if we have quads or triangles
			char *temp = contents;
			while (*temp != '\n')
			{
				if (*temp == ' ')
					spaceCount++;
				temp++;
			}

			//check for quads
			if (spaceCount == 4)
			{
				sscanf(contents, "f %d//%d %d//%d %d//%d %d//%d", &a, &id, &b, &id, &c, &id, &d, &id);
				faces.push_back(new Face(id, a, b, c, d));
			}
			//triangles
			else
			{
				sscanf(contents, "f %d//%d %d//%d %d//%d", &a, &id, &b, &id, &c, &id);
				faces.push_back(new Face(id, a, b, c));
			}
		}
		contents = moveToNextLine(contents);
	}

	//draw

	//generate a display list
	int num = glGenLists(1);

	glNewList(num, GL_COMPILE);
	for (int i = 0; i < faces.size(); i++)
	{
		//quads?
		if (faces[i]->quad)
		{
			glBegin(GL_QUADS);
			glNormal3f(normals[faces[i]->facenum - 1]->x, normals[faces[i]->facenum - 1]->y, normals[faces[i]->facenum - 1]->z); //-1 because C index from 0
			glVertex3f(vertex[faces[i]->faces[0] - 1]->x, vertex[faces[i]->faces[0] - 1]->y, vertex[faces[i]->faces[0] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[1] - 1]->x, vertex[faces[i]->faces[1] - 1]->y, vertex[faces[i]->faces[1] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[2] - 1]->x, vertex[faces[i]->faces[2] - 1]->y, vertex[faces[i]->faces[2] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[3] - 1]->x, vertex[faces[i]->faces[3] - 1]->y, vertex[faces[i]->faces[3] - 1]->z);
			glEnd();
		}
		else
		{
			glBegin(GL_TRIANGLES);
			glNormal3f(normals[faces[i]->facenum - 1]->x, normals[faces[i]->facenum - 1]->y, normals[faces[i]->facenum - 1]->z); //-1 because C index from 0
			glVertex3f(vertex[faces[i]->faces[0] - 1]->x, vertex[faces[i]->faces[0] - 1]->y, vertex[faces[i]->faces[0] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[1] - 1]->x, vertex[faces[i]->faces[1] - 1]->y, vertex[faces[i]->faces[1] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[2] - 1]->x, vertex[faces[i]->faces[2] - 1]->y, vertex[faces[i]->faces[2] - 1]->z);
			glEnd();
		}
	}
	glEndList();

	//cleanup
	for (int i = 0; i < faces.size(); i++)
		delete faces[i];
	for (int i = 0; i < normals.size(); i++)
		delete normals[i];
	for (int i = 0; i < vertex.size(); i++)
		delete vertex[i];

	//(NOTE):rewind to the beginning, can't free memory otherwise! don't know why!
	contents = contentsAddress;
	free(contents);

	return num;

}


void model(int object)
{
	glCallList(object);
}

struct Texture
{
	uint32 id = 0;
	int32 width = 0;
	int32 height = 0;
	uint32 *pixels = 0;

	
	void free()
	{
		if (id != 0)
		{
			glDeleteTextures(1, &id);
			id = 0;
		}

		//Delete pixels
		if (pixels != NULL)
		{
			delete[] pixels;
			pixels = NULL;
		}

		
		width = 0;
		height = 0;
	}
	/*int LoadGLTextures()
	{
	id = SOIL_load_OGL_texture
	(
	"data/circle.png",
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_INVERT_Y
	);

	if (id == 0)
	return false;

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
	}*/

	uint32 loadTexture(const char *filename)
	{
		int32 bpp;
		uint8 *pix = stbi_load(filename, &width, &height, &bpp, 3);

		//generate 1 texture
		glGenTextures(1, &id);

		//bind the texture id to a texture target
		glBindTexture(GL_TEXTURE_2D, id);

		//set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//linear min filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//linear mag filter
																			
		//load texture from memory buffer
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pix);

		stbi_image_free(pix);
		return id;
	}

	bool32 loadTexturePixels(uint32 *pix, uint32 w, uint32 h)
	{
		free();

		width = w;
		height = h;

		//generate 1 texture
		glGenTextures(1, &id);

		//bind the texture id to a texture target
		glBindTexture(GL_TEXTURE_2D, id);

		//set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//load texture from memory buffer
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);

		//unbind texture
		glBindTexture(GL_TEXTURE_2D, NULL);

		//Check for error
		uint32 error = glGetError();
		if (error != GL_NO_ERROR)
		{
			printf("Error loading texture from pixels: %s\n", gluErrorString(error));
			return false;
		}

		return true;
	}

	//update texture
	bool lock()
	{
		//If texture is not locked and a texture exists
		if (pixels == NULL && id != 0)
		{
			//Allocate memory for texture data
			GLuint size = width * height;
			pixels = new GLuint[size];

			//Set current texture
			glBindTexture(GL_TEXTURE_2D, id);

			//Get pixels
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			//Unbind texture
			glBindTexture(GL_TEXTURE_2D, NULL);

			return true;
		}

		return false;
	}

	bool unlock()
	{
		//If texture is locked and a texture exists
		if (pixels != NULL && id != 0)
		{
			//Set current texture
			glBindTexture(GL_TEXTURE_2D, id);

			//Update texture
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			//Delete pixels
			delete[] pixels;
			pixels = NULL;

			//Unbind texture
			glBindTexture(GL_TEXTURE_2D, NULL);

			return true;
		}

		return false;
	}

	uint32 getPixel32(uint32 x, uint32 y)
	{
		return pixels[y * width + x];
	}

	void setPixel32(uint32 x, uint32 y, uint32 pixel)
	{
		pixels[y * width + x] = pixel;
	}

	void draw()
	{
		//check if wireframe rendering is turned on
		if (state.fillFlag == false)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//enable texturing
		glEnable(GL_TEXTURE_2D);

		if (id != 0)
		{
			glBindTexture(GL_TEXTURE_2D, id);

			//place texture on quad
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
			glEnd();
		}

		glDisable(GL_TEXTURE_2D);

		//turn on noFill again
		if (state.fillFlag == false)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void draw(real32 x, real32 y, real32 w = 0, real32 h = 0)
	{
		if (id != 0)
		{
			if (w == 0)
				w = (real32)width;

			if (h == 0)
				h =(real32) height;

			//check if wireframe rendering is turned on
			if (state.fillFlag == false)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


			//enable texturing
			glEnable(GL_TEXTURE_2D);

			glLoadIdentity();
			glTranslatef(x, y, 0.f);
			glBindTexture(GL_TEXTURE_2D, id);

			//place texture on quad
			glBegin(GL_QUADS);
			glTexCoord2f(0.f, 0.f); glVertex2f(0.f, 0.f);
			glTexCoord2f(1.f, 0.f); glVertex2f(w, 0.f);
			glTexCoord2f(1.f, 1.f); glVertex2f(w, h);
			glTexCoord2f(0.f, 1.f); glVertex2f(0.f, h);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			//turn on noFill again
			if (state.fillFlag == false)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}


	
	// Draw a part of a texture (defined by a rectangle) with 'pro' parameters
	// NOTE: origin is relative to destination rectangle size
	void drawEx(Rect sourceRec, Rect destRec, vec2 origin, float rotation, Color tint)
	{
		// Check if texture is valid
		if (id != 0)
		{
			if (sourceRec.w < 0) sourceRec.x -= sourceRec.w;
			if (sourceRec.h < 0) sourceRec.y -= sourceRec.h;

			glBindTexture(GL_TEXTURE_2D, id);

			glPushMatrix();
			glTranslatef((float)destRec.x, (float)destRec.y, 0);
			glRotatef(rotation, 0, 0, 1);
			glTranslatef(-origin.x, -origin.y, 0);

			glBegin(GL_QUADS);
			glColor4ub((uint8)tint.r, (uint8)tint.g, (uint8)tint.b, (uint8)tint.a);
			glNormal3f(0.0f, 0.0f, 1.0f);                          // Normal vector pointing towards viewer

			// Bottom-left corner for texture and quad
			glTexCoord2f((float)sourceRec.x / width, (float)sourceRec.y / height);
			glVertex2f(0.0f, 0.0f);

			// Bottom-right corner for texture and quad
			glTexCoord2f((float)sourceRec.x / width, (float)(sourceRec.y + sourceRec.h) / height);
			glVertex2f(0.0f, (float)destRec.h);

			// Top-right corner for texture and quad
			glTexCoord2f((float)(sourceRec.x + sourceRec.w) / width, (float)(sourceRec.y + sourceRec.h) / height);
			glVertex2f((float)destRec.w, (float)destRec.h);

			// Top-left corner for texture and quad
			glTexCoord2f((float)(sourceRec.x + sourceRec.h) / width, (float)sourceRec.y / height);
			glVertex2f((float)destRec.w, 0.0f);
			glEnd();
			glPopMatrix();

			glDisable(GL_TEXTURE_2D);
		}
	}

	// Draw a Texture2D
	void draw(int posX, int posY, Color tint)
	{
		draw(vec2{ (float)posX, (float)posY }, 0.0f, 1.0f, tint);
	}

	// Draw a Texture2D with position defined as Vector2
	void DrawTextureV(Texture texture, vec2 position, Color tint)
	{
		draw(position, 0, 1.0f, tint);
	}

	// Draw a Texture2D with extended parameters
	void draw(vec2 position, real32 rotation, real32 scale, Color tint)
	{
		Rect sourceRec = { 0.f, 0.f, (real32)width, (real32)height };
		Rect destRec = { position.x, position.y, width*scale, height*scale };
		vec2 origin = { 0, 0 };

		drawEx(sourceRec, destRec, origin, rotation, tint);
	}

	// Draw a part of a texture (defined by a rectangle)
	void drawRect(Rect sourceRec, vec2 position, Color tint)
	{
		Rect destRec = { position.x, position.y, abs(sourceRec.w), abs(sourceRec.h) };
		vec2 origin = { 0, 0 };

		drawEx(sourceRec, destRec, origin, 0.0f, tint);
	}

	void draw(real32 x, real32 y, Rect *clip)
	{
#if 0
		Rect sourceRec = { clip->x, clip->y, clip->w, clip->h };
		Rect destRec = { (int)x, (int)y, clip->w, clip->h };
		vec2 origin = { 0, 0 };
		drawEx(sourceRec, destRec, origin, 0.0f, Color{ 0xff, 0xff, 0xff, 0xff });
#endif

		//If the texture exists
		if (id != 0)
		{
			//enable texturing
			glEnable(GL_TEXTURE_2D);

			//Remove any previous transformations
			glLoadIdentity();

			//Texture coordinates
			real32 texTop = 0.f;
			real32 texBottom = 1.f;
			real32 texLeft = 0.f;
			real32 texRight = 1.f;

			//Vertex coordinates
			real32 quadWidth = (real32)width;
			real32 quadHeight = (real32)height;

			//Handle clipping
			if (clip != NULL)
			{
				//Texture coordinates
				// turn pixel coordinates into texture mapping coordinates, take the position and divide by the pixel width(or height for the vertical position)
				texLeft = clip->x / width;
				texRight = (clip->x + clip->w) / width;
				texTop = clip->y / height;
				texBottom = (clip->y + clip->h) / height;

				//Vertex coordinates
				quadWidth = clip->w;
				quadHeight = clip->h;
			}

			//Move to rendering point
			glTranslatef(x, y, 0.f);

			//Set texture ID
			glBindTexture(GL_TEXTURE_2D, id);

			//Render textured quad
			glBegin(GL_QUADS);
			glTexCoord2f(texLeft, texTop); glVertex2f(0.f, 0.f);
			glTexCoord2f(texRight, texTop); glVertex2f(quadWidth, 0.f);
			glTexCoord2f(texRight, texBottom); glVertex2f(quadWidth, quadHeight);
			glTexCoord2f(texLeft, texBottom); glVertex2f(0.f, quadHeight);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}
};

uint32 loadTexture(const char *filename)
{
	int width, height, bpp;
	unsigned char *pix = stbi_load(filename, &width, &height, &bpp, 3);
	unsigned int id;

	//generate 1 texture
	glGenTextures(1, &id);

	//bind the texture id to a texture target
	glBindTexture(GL_TEXTURE_2D, id);

	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//linear min filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//linear mag filter

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pix);
	stbi_image_free(pix);
	return id;
}




void sprite(uint32 tex, int32 x, int32 y, int32 w, int32 h)
{
	//check if wireframe rendering is turned on
	if (state.fillFlag == false)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glLoadIdentity();
	glTranslatef((real32)x, (real32)y, 0.0f);

	glBindTexture(GL_TEXTURE_2D, tex);

	//place texture on quad
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f((real32)w, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f((real32)w, (real32)h);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(0.0f, (real32)h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	//glLoadIdentity();

	//change back the stroke color to the current selected
	glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);

	//turn on noFill again
	if (state.fillFlag == false)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void disableDoubleBuffer()
{
	doubleBufferDisabledFlag = true;
	glDrawBuffer(GL_FRONT);
}