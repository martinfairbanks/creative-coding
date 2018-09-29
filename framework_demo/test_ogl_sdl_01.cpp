#define OPENGL
#include "..\framework\creativeframework.cpp"

real32 angle = 0;
real32 zPos = 0;
uint32 texture = 0;
int32 myModel = 0;

void setup()
{
	screen(960, 540, false, "creative coding");
	texture = loadTexture("data/pix/karate.bmp");
	myModel = loadModel("data/3d/cube.obj");
}

void updateAndDraw()
{
	clear(c64blue);
	set2dProjection();

	enableFog(-5.0f, 5.0f, 0.4f, 0.4f, 0.4f, 1.0f);
	sprite(texture, 200, 100, 200, 200);
	disableFog();

	strokeWeight(4);
	stroke(blue);
	fill(green);
	line(150, 50, 250, 50);
	circle(300, 50, 50);
	fill(1.0f, 0, 0, 0.5f);
	circle(350, 50, 10);
	rect(500, 50, 50, 50);
	stroke(green);
	fill(magenta);
	ellipse(600, 200, 80, 100);
	stroke(blue);
	point(windowWidth / 2, windowHeight / 2);
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
	stroke(black);
	strokeWeight(2);
	fill(255, 0, 150);
	lights();
	translate(0, 0, -200.f);
	//translate(mouseX - windowWidth/2, abs(mouseY-windowHeight) - windowHeight/2);
	//rectMode(CENTER);
	rotateX(-angle);
	rotateY(-angle*0.3f);
	rotateZ(-angle*0.8f);

	//box(50,50,20);
	torus(50,10);

	angle += 0.7f;

	rectMode(CENTER);
	fill(blue);
	rect(0,0,150,100);
	fill(0, 0, 150);
	translate(0, 0, -1.f);
	circle(0,0,50);
	fill(red);
	box(10, 10, 50);
	
	rotateX(angle);
	rotateY(angle*2.3f);
	rotateZ(angle*1.2f);	

	stroke(pink);
	fill(black);
	loadIdentity();
	pushMatrix();
		translate(0.0f, 0.0f, -10.0f);
		rotateY(angle);
		rotateX(angle);
		model(myModel);
	popMatrix();

	loadIdentity();
	translate(0.0f, 0.0f, zPos);
	rotateX(angle);

	//draw pyramid
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);		//top - front
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);		//left
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);		//right

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);		//top - right
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);		//left 
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);		//right

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);		//top - back
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);		//left
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);	//right

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);		//top - left
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);	//left
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);		//right 
	glEnd();

	zPos = 4.0f * ((real32)cos(angle / 20.0f) - 1.5f);
}

void shutdown() { }