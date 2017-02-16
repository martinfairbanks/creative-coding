	#define OPENGL4	
	#include "framework\creativeframework.cpp"

void setup()
{
	/* the vertex shader positions each vertex point */
	vertexShaderSource =	"#version 430 core														\n"
							"layout(location = 0) in vec4 in_position;								\n"
							"layout (location = 1) in vec4 color;									\n"
							"out vec4 vs_color;														\n"
							"void main(void)														\n"
							"{																		\n"
							"    gl_Position = in_position;											\n"
							"    vs_color = color;													\n"
							"}																		\n";

	/* the fragment shader colours each fragment (pixel-sized area of the triangle) */
	fragmentShaderSource =	"#version 430 core														\n"
							"																		\n"
							"in vec4 vs_color;														\n"
							"out vec4 color;														\n"
							"																		\n"
							"void main(void)														\n"
							"{																		\n"
							"    color = vs_color;													\n"
							"}																		\n";

	screen(960, 540, false, "opengl framework");
	
	
}

void drawTriangle(vec2 v1, vec2 v2, vec2 v3)
{
	float32 verts[] =
	{
		v1.x, v1.y,
		v2.x, v2.y,
		v3.x, v3.y
	};

	GLuint bufferID;
	///generate 1 vertex buffer
	glGenBuffers(1, &bufferID);
	//bind the buffer to the GL_ARRAY_BUFFER binding point 
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	//upload the buffer data to the GPU - send the data to the buffer that is bound to the GL_ARRAY_BUFFER binding point
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	//enable the data that was copied to the buffer to go to through the graphics processing pipeline 
	//enable the position attribute
	glEnableVertexAttribArray(0);
	//tells opengl how our data buffer is structured -  - point it to the start of the buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void square(float32 size)
{
	glPointSize(size);
	glDrawArrays(GL_POINTS, 0, 1);
}

vec3 linePoints[2];
void line(float x1, float y1, float z1, float x2, float y2, float z2) 
{
	linePoints[0] = vec3(x1, y1, z1);
	linePoints[1] = vec3(x2, y2, z2);

	
	GLuint bufferID;
	///generate 1 vertex buffer
	glGenBuffers(1, &bufferID);
	//bind the buffer to the GL_ARRAY_BUFFER binding point 
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	//upload the buffer data to the GPU - send the data to the buffer that is bound to the GL_ARRAY_BUFFER binding point
	glBufferData(GL_ARRAY_BUFFER, sizeof(float32) * 3 * 2, &linePoints[0].x, GL_STATIC_DRAW);
	//enable the data that was copied to the buffer to go to through the graphics processing pipeline 
	//enable the position attribute
	glEnableVertexAttribArray(0);
	//tells opengl how our data buffer is structured -  - point it to the start of the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 2);
}

void updateAndDraw(uint32 t)
{
	clear(0.2f, 0.3f, 0.3f);
	noFill();
	setColor(green);
	square(200);
	setColor(red);
	drawTriangle(vec2{ 0.0, 0.5 }, vec2{ -1.0f, -1.0f }, vec2{ 1.0f, -1.0f });
	setColor(blue);
	line(0.0f, 0.0f, 1.f, 0.5f, -0.3f, 1.f);

	//glPointSize(40.0f);
	//use shader program for rendering
	//set wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/*colorShader.enable();
	GLfloat offset = -0.9f;
	glUniform1f(glGetUniformLocation(colorShader.programID, "xOffset"), offset);

	drawTriangle(Vector2d{ 0.0, 0.5 }, Vector2d{ -1.0f, -1.0f }, Vector2d{ 1.0f, -1.0f });
	
	drawTriangle(Vector2d{ 0.0, 0.0 }, Vector2d{ -1.0f, -1.0f }, Vector2d{ 1.0f, -1.0f });
	setColor(Color::blue);
	square(200);
	colorShader.disable();*/
	//colorShader.enable();
	//glBindVertexArray(vao);
	
	
	//draw texture
	//glBindVertexArray(vao);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);

	//glBindVertexArray(vbo);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindVertexArray(0);
	//textureShader.disable();
}

void shutdown() { }
