
	//uint32 vbo;	//vertex buffer object id
	//uint32 vao;	//vertex array object id
	char *vertexShaderSource = 0;
	char *fragmentShaderSource = 0;

void clear(real32 r, real32 g, real32 b, real32 a = 1.f)
{
	const real32 col[] = { r, g, b, a };
	glClearBufferfv(GL_COLOR, 0, col);
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
}

void clear(Color col)
{
	const real32 c[] = { (real32)col.r, (real32)col.g, (real32)col.b, (real32)col.a };
	glClearBufferfv(GL_COLOR, 0, c);
}

struct Shader
{
	//how many attributes to use ->0 = position, 1 = color etc.
	uint32 numAttributes = 0;
	uint32 programID = 0;
	uint32 vertexShaderID = 0;
	uint32 fragmentShaderID = 0;

	void loadFileShader(char *vertexShader, char *fragmentShader)
	{
		programID = glCreateProgram();

		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (vertexShaderID == 0)
		{
			printLogError("Vertex shader failed to be created!");
		}

		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (fragmentShaderID == 0)
		{
			printLogError("Fragment shader failed to be created!");
		}

		char *vs = loadTextFile(vertexShader);
		char *fs = loadTextFile(fragmentShader);
		if (vs)
			compileShader(vs, vertexShaderID);
		else
			printLogError("Failed to load vertex shader!");
		if (fs)
			compileShader(fs, fragmentShaderID);
		else
			printLogError("Failed to load fragment shader!");

		free(vs);
		free(fs);
	}

	void loadShader(char *vertexShader, char *fragmentShader)
	{
		programID = glCreateProgram();

		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (vertexShaderID == 0)
		{
			printLogError("Vertex shader failed to be created!");
		}

		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (fragmentShaderID == 0)
		{
			printLogError("Fragment shader failed to be created!");
		}

		compileShader(vertexShader, vertexShaderID);
		compileShader(fragmentShader, fragmentShaderID);
	}

	void linkShaders()
	{
		//link shaders to make the shaderprogram
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		//check for linking errors
		int32 success = 0;
		char log[512];
		glGetProgramiv(programID, GL_LINK_STATUS, (int *)&success);
		if (!success)
		{
			glGetProgramInfoLog(programID, 512, NULL, log);
			glDeleteProgram(programID);
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
			printLogError("Error when linking shaders: %s\n", log);
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	//adds a attribute to the shader - what we want the shader to process, position, colors, textures
	void addAttribute(char *attributeName)
	{
		glBindAttribLocation(programID, numAttributes, attributeName);
		numAttributes++;
	}

	int32 getUniformLocation(char* uniformName)
	{
		int32 location = glGetUniformLocation(programID, uniformName);

		if (location == GL_INVALID_INDEX)
		{
			printLogError("Uniform %s not found in shader!", uniformName);
		}
		return location;
	}
	
	void setBool(char *name, bool32 value)
	{         
		int32 location = getUniformLocation(name);
		glUniform1i(location, value);
	}

	void setInt(char *name, int32 value)
	{ 
		int32 location = getUniformLocation(name);
		glUniform1i(location, value);
	}

	void setFloat(char *name, real32 value)
	{ 
		int32 location = getUniformLocation(name);
		glUniform1f(location, value);
	}

	void enable()
	{
		glUseProgram(programID);
		//enable attributes
		for (int i = 0; i < numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void disable()
	{
		glUseProgram(0);
		for (int i = 0; i < numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}
	
	void compileShader(const char *contents, uint32 id)
	{
		glShaderSource(id, 1, &contents, 0);
		glCompileShader(id);

		//check for compilation errors
		int32 success = 0;
		char log[512];
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, log);
			glDeleteShader(id);
		//	printLogError("Shader %s error: %s\n", filename, log);
			printLogError("Shader error: %s\n", log);
		}
	}
};

Shader colorShader, textureShader;

void stroke(real32 r, real32 g, real32 b, real32 a = 1.f)
{
	strokeColor = { r, g, b, a };
	glUseProgram(colorShader.programID);
	real32 c[] = { r, g, b, 255 };
	// Update the value of input attribute 0
	glVertexAttrib4fv(1, c);
}

void setColor(Color col)
{
	real32 r = real32(col.r / 255.0);
	real32 g = real32(col.g / 255.0);
	real32 b = real32(col.b / 255.0);
	strokeColor.r = r;
	strokeColor.g = g;
	strokeColor.b = b;
	strokeColor.a = 1.0f;

	glUseProgram(colorShader.programID);
	real32 c[] = { strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a };
	// Update the value of input attribute 0
	glVertexAttrib4fv(1, c);
}

void fill()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void noFill()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

struct GLTexture
{
	GLuint id;
	int width;
	int height;
};

struct Position
{
	real32 x;
	real32 y;
};

struct GLColorRGBA
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

//UV coordinates for texture mapping
struct UV
{
	float u;
	float v;
};

struct Vertex
{
	Position pos;
	GLColorRGBA color;
	UV uv;

	void setPosition(float x, float y)
	{
		pos.x = x;
		pos.y = y;
	}

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.b = a;
	}

	void setUV(float u, float v)
	{
		uv.u = u;
		uv.v = v;
	}
};

GLTexture loadTexture(char *filename)
{
	//init to 0
	GLTexture texture = {};
	
	//generate 1 texture
	glGenTextures(1, &(texture.id));

	//bind the texture
	//all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	glBindTexture(GL_TEXTURE_2D, texture.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//upload the imgae data to the texture -> upload the pixels to the VRAM on the gfx card
	int width, height, bpp;
	unsigned char *image = stbi_load(filename, &width, &height, &bpp, 3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//mipmapping = when the texture becomes smaller it dosen't use all the pixels to display the image
	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(image);

	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;

	return texture;
}

struct Sprite
{
	vec2 pos{0,0};
	int32 width;
	int32 height;
	//vertex buffer object to hold vertices
	uint32 vboID;

	Sprite()
	{
		//sets the vertex buffer to zero
		vboID = 0;
	}

	~Sprite()
	{
		//delete the buffer
		if (vboID != 0)
		{
			glDeleteBuffers(1, &vboID);
		}
	}

	void create(real32 x, real32 y, real32 w, real32 h)
	{
		pos.x = x;
		pos.y = y;
		width = w;
		height = h;

		if (vboID == 0)
		{
			//generate vertex buffer
			glGenBuffers(1, &vboID);
		}

		//six vertices for a square
		Vertex vertexData[6];

		//first triangle
		vertexData[0].setPosition(x + width, y + height); //top right corner
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//second triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);

		//set colors
		for (int i = 0; i < 6; i++)
		{
			vertexData[i].setColor(255, 0, 255, 255);
		}

		//angle++;
		vertexData[1].setColor(0, 0, 255, 255);
		vertexData[4].setColor(0, 255, 0, 255);

		//bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		//upload the buffer data
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	}

	void draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		//enable the position attribute
		glEnableVertexAttribArray(0);

		//position attribute pointer. tells opengl how our data buffer is structured - point it to the start of the buffer
		//start at 0 using 2 floats
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));

		//color attribute pointer, use normalize = convert numbers to a range from 0 to 1
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));

		//UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
	}
};
