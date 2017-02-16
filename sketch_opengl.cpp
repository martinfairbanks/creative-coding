#define OPENGL

#include "framework\creativeframework.cpp"
	
float angle = 0;
//Ambient light, directionless light, reddish hue over all surfaces
float amb[] = { 2.0, 0.0, 0.0, 1.0 };
//Diffuse light, white light from a direction, lamp, candle etc
float dif[] = { 1.0,1.0,1.0,1.0 };
//Specular light. "Like diffuse light, specular light is a highly directional property, but it interacts more
//sharply with the surface and in a particular direction."

float spec[] = { 0.99, 0.0, 0.0, 1.0 };

void setup()
{
	screen(960, 540, false, "opengl framework");
	set2dProjection();
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

}

void light(float32 x, float32 y, float32 z, float32 a)
{
	//Enable Lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Enable color material
	//Annars ingen färg på kuben
	glEnable(GL_COLOR_MATERIAL);

	//Position/Direction of light
	//float32 pos[] = { -2.0,2.0,-3.0,1.0 };
	float32 pos[] = { x,y,z,1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

}
void pointLight(float32 x, float32 y, float32 z, float32 a)
{
	GLfloat	 lightPos[] = { x, y, z, a }; //{ -60, 55, -240, 0.0f };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	
	// Light values and coordinates
	/*GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat  diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat  specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat	 lightPos[] = { -100.0f, 55.0f, -20.0f, 1.0f }; //{ -60, 55, -240, 0.0f };
	GLfloat  specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat	 noLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat lightPosMirror[4] = { -100.0f, -100.0f, 50.0f, 1.0f };*/

	// Setup light parameters
	/*glLightModelfv(GL_LIGHT_MODEL_AMBIENT, noLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);*/
	

	// Mostly use material tracking
	glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glMateriali(GL_FRONT, GL_SHININESS, 128);
	// Enable color tracking
	//glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	//glMateriali(GL_FRONT, GL_SHININESS, 128);

}
void ambientLight(float32 x, float32 y, float32 z, float32 a)
{
	//Enable Lighting
	glEnable(GL_LIGHTING);

	//Ambient light, directionless light, dark white light
	float aLight[] = { 0.5,0.5,0.5,1.0 };

	// Bright white light – full intensity RGB values
	//Ambient light, directionless light, dark white light
	//float aLight[] = { 1.0, 1.0, 1.0, 1.0 };

	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	// Set light model to use ambient light specified by ambientLight[]
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, aLight);

	// Enable Material color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Front material ambient and diffuse colors track glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void directionalLight(float32 x, float32 y, float32 z, float32 a)
{
	//Enable Lighting
	glEnable(GL_LIGHTING);

	//Ambient light, directionless light, dark white light
	float ambientLight[] = { 0.5,0.5,0.5,1.0 };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	// Set light model to use ambient light specified by ambientLight[]
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	// Enable Material color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Front material ambient and diffuse colors track glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}
void updateAndDraw(uint32 t)
{
	
	//pointLight(mouseX, mouseY, 10.0f, 1.0f);
	
	clear(teal);
	set2dProjection();/*
	clear(Color::teal);
	setLineWidth(2);
	stroke(Color::blue);
	fill(Color::green);
	line(50, 50, 400, 300);
	circle(screenWidth / 2, screenHeight / 2, 250);
	rect(600, 30, 50, 50);
	stroke(Color::pink);
	fill(Color::magenta);
	ellipse(300, 400, 80, 100);
	stroke(Color::red);
	pixel(screenWidth / 2, screenHeight / 2);
	//noFill();
	circle(10, 40, 50);
	rect(20, 60, 50, 50);*/

	
	static GLfloat rot = 0.0f;
	
	set3dProjection();
	stroke(pink);
	//ambientLight(mouseX, mouseY, 10.0f, 1.0f);
	light(mouseX, 2.0, 3.0, 1.0);
/*	pushMatrix();
		//plane(15.0, 2.0f);
		glShadeModel(GL_SMOOTH);
		glBegin(GL_TRIANGLES);
		// Red Apex
		translate(0.0f, 0.0f, -5.0f);
		glColor3ub((GLubyte)255, (GLubyte)0, (GLubyte)0);
		glVertex3f(0.0f, -0.5f, 4.0f);
		// Green on the right bottom corner
		glColor3ub((GLubyte)0, (GLubyte)255, (GLubyte)0);
		glVertex3f(-0.5f, 0.2f, 4.0f);
		// Blue on the left bottom corner
		glColor3ub((GLubyte)0, (GLubyte)0, (GLubyte)255);
		glVertex3f(0.5f, 0.2f, 4.0f);
		glEnd();
	popMatrix();*/
	//glShadeModel(GL_FLAT);
	stroke(blue);
	pushMatrix();
		translate(0.0f, 0.0f, 2.0f);
		rotateX(t / 10);
		rotateY(t / 10);
		//torus(0.5, 0.5, 61, 37);
	popMatrix();
	stroke(magenta);
	rot += 0.5f;
	pushMatrix();
		translate(0.0f, 0.0f, 2.5f);
		pushMatrix();
			rotateY(-rot * 2.0f);;
			translate(1.0f, 0.0f, 0.0f);
			sphere(0.4f, 17, 9);
		popMatrix();

		stroke(1.0f, 0.0f, 1.0f);
		rotateX(t/10);
		rotateY(t/10);
		//box(1.5f, 0.2f, 1.0f);
		
		cube(1.0f);
	popMatrix();

	
}


void shutdown() { }



/*
set3dProjection();
fill();
angle += 1;

//reset the modelview matrix, sets x,y,z to zero
glLoadIdentity();

glTranslatef(0.0f, 0.0f, -10.0f);
glRotatef(angle, 0.0f, 0.0f, 0.5f);
glScalef(1.0f, 1.0f, 1.0f);

//Begin triangle coordinates
glBegin(GL_TRIANGLES);
glColor3f(1.0, 0.0, 0.0);
glVertex3f(0.0f, 2.0f, 0.0f);
glColor3f(0.0, 1.0, 0.0);
glVertex3f(-2.0f, -2.0f, 0.0f);
glColor3f(0.0, 0.0, 1.0);
glVertex3f(2.0f, -2.0f, 0.0f);
glEnd();

set2dProjection();*/