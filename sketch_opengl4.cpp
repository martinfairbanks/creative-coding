	//#define SDL2	
	//#define WIN_32	
	#include "framework\creativeframework.cpp"

void setup()
{
	screen(960, 540, false, "opengl framework");
}

void updateAndDraw(uint32 t)
{
	//const real32 color[] = { 0.2f, 0.3f, 0.3f, 1.0f };
	//glClearBufferfv(GL_COLOR, 0, color);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//use shader program for rendering
	glUseProgram(shaderProgram);

	//set wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(vbo);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void shutdown() { }
