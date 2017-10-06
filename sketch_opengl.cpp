#define OPENGL
#include "framework\creativeframework.cpp"

/* TODO:	fix cylinder
			light functions
			camera
			skybox
*/

float angle = 0;
unsigned int texture = 0;
int myModel = 0;

void setup()
{
	screen(1024, 768, false, "opengl framework");
	texture = loadTexture("data/karate.bmp");
	myModel = loadModel("data/cube.obj");
	set2dProjection();

	float pos[] = { -1.0, 1.0, 4.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

}

void updateAndDraw(uint32 t)
{
	set2dProjection();
	
	enableFog(-5.0f, 5.0f, 0.4f, 0.4f, 0.4f, 1.0f);
	clear(black);
	sprite(texture, 200, 100, 200, 200);
	disableFog();

	strokeWeight(4);
	stroke(blue);
	fill(green);
	line(150, 50, 250, 50);
	circle(300, 50, 50);
	fill(1.0f,0,0,0.5f);
	circle(350, 50, 10);
	rect(500, 50, 50, 50);
	stroke(green);
	fill(magenta);
	ellipse(600, 200, 80, 100);
	stroke(blue);
	pixel(screenWidth / 2, screenHeight / 2);
	noFill();
	circle(100, 400, 50);
	rect(100, 500, 50, 50);
	fill(blue);
	stroke(red);
	noStroke();
	quad(30, 30, 100, 30, 80, 100, 50, 100);
	strokeWeight(10);
	stroke(yellow);
	line(290, 250, 450, 150);

	set3dProjection();
	stroke(0.0f, 1.0f, 0.0f);
	noFill();
	
	pushMatrix();
		translate(-3.5f, 1.0f, 0.0f);
		rotateX(angle);
		sphere(1.2f);
	popMatrix();

	pushMatrix();
		translate(0.0f, 2.0f, 0.0f);
		rotateY(angle);
		rotateX(angle);
		cylinder(1.2f, 1.2f);
	popMatrix();

	pushMatrix();
		translate(3.0f, 1.0f, 0.0f);
		rotateY(angle);
		rotateX(angle);
		cone(1.0f, 2.0f);
	popMatrix();

	pushMatrix();
		translate(0.0f, -2.0f, 0.0f);
		rotateY(angle);
		rotateX(angle);
		torus(1.5f, 0.5f);
	popMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	
	stroke(pink);
	fill(black);
	pushMatrix();
		translate(0.0f, 0.0f, 2.0f);
		rotateY(angle);
		rotateX(angle);
		model(myModel);
	popMatrix();
	
	glDisable(GL_LIGHTING);
	angle += 0.8;

}


void shutdown() { }

