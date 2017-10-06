/* Linking */
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <SDL2/SDL.h>
#include <sdl2/SDL_opengl.h>
#include <gl/glu.h>
#include <crtdbg.h> //memory leak check
#include <map>		//for keyPressed

#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb_image.h"

/* Globals */
SDL_GLContext glContext;
SDL_Window *window = 0;
SDL_GameController *controllerHandle;
const uint8 *keystates = 0;
std::map<int, bool> keyArray;

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

inline void fill(float32 r, float32 g, float32 b, float32 a = 1.f)
{
	fillFlag = true;
	fillColor.r = r;
	fillColor.g = g;
	fillColor.b = b;
	fillColor.a = a;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
inline void fill(ColorRGBA col)
{
	fillFlag = true;
	fillColor = col;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

inline void noFill()
{
	fillFlag = false;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

//clear screen buffer
void clear(float32 col)
{
	glClearColor(col, col, col, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clear(float32 r, float32 g, float32 b, float32 a = 1.f)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clear(ColorRGBA col)
{
	glClearColor(col.r, col.g, col.b, col.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void strokeWeight(int32 value)
{
	lineWidth = value;
	glLineWidth((float32)value);
}

void noStroke()
{
	lineWidth = 0;
	glLineWidth(1);
}

void stroke(float32 r, float32 g, float32 b, float32 a = 1.f)
{
	strokeColor.r = r;
	strokeColor.g = g;
	strokeColor.b = b;
	strokeColor.a = a;
	glColor4f(r, g, b, a);
}

void stroke(ColorRGBA col)
{
	strokeColor = col;
	glColor4f(col.r, col.g, col.b, col.a);
}

inline void line(int32 x0, int32 y0, int32 x1, int32 y1)
{
	glBegin(GL_LINES);
	glVertex2i(x0, y0);
	glVertex2i(x1, y1);
	glEnd();
}

inline void pixel(int32 x, int32 y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

inline void rect(int32 x, int32 y, int32 width, int32 height)
{
	//glRectf(50.0f, 50.0f, 25.0f, 25.0f);
	if (fillFlag)
	{
		glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
		glBegin(GL_QUADS);
		glVertex2i(x, y);
		glVertex2i(x + width, y);
		glVertex2i(x + width, y + height);
		glVertex2i(x, y + height);
		glEnd();
		glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	}
	
	if (lineWidth > 0)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2i(x, y);
		glVertex2i(x + width, y);
		glVertex2i(x + width, y + height);
		glVertex2i(x, y + height);
		glEnd();
	}
}

void quad(int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, int32 x4, int32 y4)
{
	if (fillFlag)
	{
		glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
		glBegin(GL_QUADS);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);
		glVertex2f(x4, y4);
		glEnd();
		glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	}
	if (lineWidth > 0)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);
		glVertex2f(x4, y4);
		glEnd();
	}
}

inline void circle(int32 x, int32 y, int32 radius)
{
	if (fillFlag)
	{
		glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2i(x, y);
		for (float32 angle = 0; angle < 360; angle++)
		{
			glVertex2f((float32)x + sin(angle) * radius, (float32)y + cos(angle) * (float32)radius);
		}
		glEnd();
		glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	}

	if (lineWidth > 0)
	{
		glBegin(GL_LINE_STRIP);
		for (float angle = 0; angle < PI32*4; angle += (PI32 / 50.0f))
		{
			glVertex2f((float32)x + sin(angle) * (float32)radius, (float32)y + cos(angle) * (float32)radius);
		}
		glEnd();
	}
}

inline void ellipse(int32 x, int32 y, int32 r1, int32 r2)
{
	if (fillFlag)
	{
		glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2i(x, y);
		for (int angle = 0; angle < 360; angle++)
		{
			glVertex2f(float32(x + sin(angle) * r1), float32(y + cos(angle) * r2));
		}
		glEnd();
		glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	}

	if (lineWidth > 0)
	{
		glBegin(GL_LINE_STRIP);
		for (float angle = 0; angle < PI32 * 4; angle += (PI32 / 50.0f))
		{
			glVertex2f((float32)x + sin(angle) * (float32)r1, (float32)y + cos(angle) * (float32)r2);
		}
		glEnd();
	}
}

void pushMatrix()
{
	glPushMatrix();
}

void popMatrix()
{
	glPopMatrix();
}

void translate(float32 x, float32 y, float32 z)
{
	glTranslatef(x, y, z);
}


void rotateX(float32 angle)
{
	glRotatef(angle, 1.0f, 0.0f, 0.0f);
}

void rotateY(float32 angle)
{
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
}

void rotateZ(float32 angle)
{
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
}

void plane(float32 width, float32 height)
{
	glBegin(GL_QUADS);
		//glNormal3f(0.0, 0.0, 1.0);
		//glColor3ub((GLubyte)col.r, (GLubyte)col.g, (GLubyte)col.b);
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(width / 2, height / 2, 1.0f);
		glVertex3f(-width / 2, height / 2, 1.0f);
		glVertex3f(-width / 2, -height / 2, 1.0f);
		glVertex3f(width / 2, -height / 2, 1.0f);
	glEnd();
}

void sphere(GLfloat radius, GLint slices = 24, GLint stacks = 16)
{
#if 1
	GLfloat drho = PI32 / (GLfloat)stacks;
	GLfloat dtheta = 2.0f * PI32 / (GLfloat)slices;
	GLfloat ds = 1.0f / (GLfloat)slices;
	GLfloat dt = 1.0f / (GLfloat)stacks;
	GLfloat t = 1.0f;
	GLfloat s = 0.0f;

	for (int32 i = 0; i < stacks; i++)
	{
		GLfloat rho = (GLfloat)i * drho;
		GLfloat srho = (GLfloat)(sin(rho));
		GLfloat crho = (GLfloat)(cos(rho));
		GLfloat srhodrho = (GLfloat)(sin(rho + drho));
		GLfloat crhodrho = (GLfloat)(cos(rho + drho));

		glBegin(GL_TRIANGLE_STRIP);
		s = 0.0f;
		for (int32 j = 0; j <= slices; j++)
		{
			GLfloat theta = (j == slices) ? 0.0f : j * dtheta;
			GLfloat stheta = (GLfloat)(-sin(theta));
			GLfloat ctheta = (GLfloat)(cos(theta));

			GLfloat x = stheta * srho;
			GLfloat y = ctheta * srho;
			GLfloat z = crho;

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

void torus(GLfloat majorRadius, GLfloat minorRadius)
{
	GLint numMajor = 61;
	GLint numMinor = 37;

	vec3 normal;
	GLfloat vNormal[3];
	double majorStep = 2.0f*PI32 / numMajor;
	double minorStep = 2.0f*PI32 / numMinor;
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
void cylinder(GLfloat width, GLfloat height)
{
	GLUquadricObj *q = gluNewQuadric();
	gluCylinder(q, width, width, height, 32, 7);
	/*glBegin(GL_TRIANGLE_FAN);
	for (float32 angle = 0; angle < 360; angle++)
	{
		glVertex2f(sin(angle) * width, cos(angle) * width);
	}
	glEnd();
	translate(0.0f, height*2, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	for (float32 angle = 0; angle < 360; angle++)
	{
		glVertex2f(sin(angle) * width, cos(angle) * width);
	}
	glEnd();*/
	gluDeleteQuadric(q);
}

void cone(GLfloat width, GLfloat height)
{
	GLUquadricObj *q = gluNewQuadric();
	gluCylinder(q, width, 0, height, 32, 7);

	glBegin(GL_TRIANGLE_FAN);
	for (float32 angle = 0; angle < 360; angle++)
	{
		glVertex2f(sin(angle) * width, cos(angle) * width);
	}
	glEnd();

	gluDeleteQuadric(q);
}

void cube(float32 size)
{
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	//front face
	//glColor3ub((GLubyte)col.r, (GLubyte)col.g, (GLubyte)col.b);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glVertex3f(size / 2, -size / 2, size / 2);

	//left face
	glNormal3f(-1.0, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);

	//back face
	glNormal3f(0.0, 0.0, -1.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);

	//right face
	glNormal3f(1.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glVertex3f(size / 2, size / 2, size / 2);
	glVertex3f(size / 2, -size / 2, size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);

	//top face
	glNormal3f(0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glVertex3f(size / 2, size / 2, -size / 2);

	//bottom face
	glNormal3f(0.0, -1.0, 0.0);
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();
}

void pyramid(float32 size)
{
	//draw pyramid
	glBegin(GL_TRIANGLES);
		//front
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);		//top
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-size, -size, size);     //left
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(size, -size, size);      //right

		//right
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(size, -size, size);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(size, -size, -size);

		//back
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(size, -size, -size);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-size, -size, -size);

		//left
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-size, -size, -size);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-size, -size, size);
	glEnd();

	//bottom of pyramid
	glBegin(GL_QUADS);
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(size, -size, size);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-size, -size, size);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-size, -size, -size);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(size, -size, -size);
	glEnd();
}

void set2dProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, screenWidth, screenHeight, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
}

void set3dProjection()
{
	//set viewport to window dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	//switch to the projection matrix and reset it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//set camera perspective
	gluPerspective(90.0f,								//camera angle, field of view in degrees, set to 90 degrees viewing angle
		(GLfloat)screenWidth / (GLfloat)screenHeight,	//aspect ratio
		1.0f,											//near z clipping coordinate
		500.0f);										//far z clipping coordinate
														//1.0f - 500.0f is the start and end point for how deep we can draw into the screen

	//switch to GL_MODELVIEW, tells OGL that all future transformations will affect what we draw
	//reset the modelview matrix, wich is where the object information is stored, sets x,y,z to zero
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//enable depth buffer
	glEnable(GL_DEPTH_TEST);

	translate(0.0f, 0.0f, -5.0f);
}

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

void enableFog(float32 start, float32 end, float32 red, float32 green, float32 blue, float32 alpha)
{
	//fog setup
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, /*GL_EXP2*/GL_LINEAR);		//fog equation
													//glFogf(GL_FOG_DENSITY, 0.6f); //funkar bara om GL_FOG_MODE == GL_EXP eller GL_EXP2
													//Note that GL_FOG_START and GL_FOG_END only have an effect on GL_LINEAR fog
													//where the fog starts and ends
	glFogf(GL_FOG_START, start);
	glFogf(GL_FOG_END, end);
	float32 color[] = { red, green, blue, alpha };
	glFogfv(GL_FOG_COLOR, color);
}

void disableFog()
{
	glDisable(GL_FOG);
}

#include <vector>

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

int loadModel(const char *filename)
{
	std::vector<vec3*> vertex;
	std::vector<Face*> faces;
	std::vector<vec3*> normals;

	char *contents = loadFile(filename);
	char *contentsAddress = contents;

	while (*contents != '\0')
	{
		if (*contents == 'v' && *(1 + contents) == ' ')
		{
			float tmpx, tmpy, tmpz;
			sscanf(contents, "v %f %f %f", &tmpx, &tmpy, &tmpz);
			vertex.push_back(new vec3(tmpx, tmpy, tmpz));
		}

		//parse the normals
		else if (*contents == 'v' && *(1+contents) == 'n')
		{
			float tmpx, tmpy, tmpz;
			sscanf(contents, "vn %f %f %f", &tmpx, &tmpy, &tmpz);
			normals.push_back(new vec3(tmpx, tmpy, tmpz));
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

unsigned int loadTexture(const char *filename)
{
	int width, height, bpp;
	unsigned char *pixels = stbi_load(filename, &width, &height, &bpp, 3);
	unsigned int id;

	//tells OpenGL we want to generate 1 texture
	glGenTextures(1, &id);

	//tells OpenGL to bind the named texture to a texture target
	glBindTexture(GL_TEXTURE_2D, id);

	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//linear min filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//linear mag filter

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	stbi_image_free(pixels);
	return id;
}

void sprite(uint32 tex, int32 x, int32 y, int32 w, int32 h)
{
	//check if wireframe rendering is turned on
	if (fillFlag == false)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glLoadIdentity();
	glTranslatef((float32)x, (float32)y, 0.0f);

	glBindTexture(GL_TEXTURE_2D, tex);

	//place texture on quad
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f((float32)w, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f((float32)w, (float32)h);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, (float32)h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();

	//change back the stroke color to the current selected
	glColor4f(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);

	//turn on noFill again
	if (fillFlag == false)
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

	uint32 flags = SDL_WINDOW_OPENGL;// | SDL_WINDOW_RESIZABLE;

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

	set3dProjection();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

	const float64 MAX_FRAME_TIME = 1000 / 60;
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
				//get new dimensions and repaint on window size change  
				case SDL_WINDOWEVENT:
				{
					if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						screenWidth = event.window.data1;
						screenHeight = event.window.data2;
						set3dProjection();
					}
				} break;
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

		mouseReleased = false;
		for (int i = 1; i <= 3; i++)
		{
			mouseButton[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);

			if (mouseIsPressed && mouseButton[i])
			{
				mouseReleased = false;
			}
			else if (mouseIsPressed && !mouseButton[1] && !mouseButton[2] && !mouseButton[3])
			{
				mouseReleased = true;
			}


		}

		mouseIsPressed = false;
		for (int i = 1; i <= 3; i++)
		{
			mouseButton[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
			if (mouseButton[i])
			{
				//mouseClick = true;
				mouseIsPressed = true;
			}
		}
		if (mouseIsPressed && (mouseX != prevMouseX || mouseX != prevMouseX))
			mouseDragged = true;
		else
			mouseDragged = false;

		/* the time it takes to draw one frame */
		uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameTime > MAX_FRAME_TIME)
			frameTime = (uint32)MAX_FRAME_TIME;
		frameStart = SDL_GetTicks();

		updateAndDraw(frameStart);
		//update screen
		SDL_GL_SwapWindow(window);

		uint64 endCycleCount = __rdtsc();
		uint64 endCounter = SDL_GetPerformanceCounter();
		uint64 counterElapsed = endCounter - lastCounter;
		uint64 cyclesElapsed = endCycleCount - lastCycleCount;
		float64 msPerFrame = (((1000.0f * (float64)counterElapsed) / (float64)performanceFrequency));
		float64 fps = (float64)performanceFrequency / (float64)counterElapsed;
		float64 megaCyclesPerFrame = ((float64)cyclesElapsed / (1000.0f * 1000.0f));
		lastCycleCount = endCycleCount;
		lastCounter = endCounter;

#if defined(_DEBUG)        
		char message[256];
		sprintf_s(message, "%.03fms, %.03fFPS, %.03fMEGAc/f, RefreshRate: %d\0", msPerFrame, fps, megaCyclesPerFrame, refreshRate);
		//print(message, 1, screenHeight - 20);
		SDL_SetWindowTitle(window, message);
		totalFrames++;
#endif
		
	}
	quit();
	return 0;
}


