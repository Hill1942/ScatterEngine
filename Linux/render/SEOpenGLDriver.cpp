#include <GL/glew.h>
#include <GL/gl.h>
#include "SEOpenGLDriver.h"



void InitGL()
{
	
	
}



GLuint CompileShaders()
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;

	static const GLchar* vertexShaderSource[] = 
	{
		"#version 140                               \n"
		"                                           \n"
		"in vec4 mypos;                             \n"
		"void main()                                \n"
		"{                                          \n"
		"	gl_Position = mypos;                    \n"
		"}                                          \n"
	};
	static const GLchar* fragmentShaderSource[] = 
	{
		"#version 140                              \n"
		"                                          \n"
		"out vec4 color;                           \n"
		"                                          \n"
		"void main()                               \n"
		"{                                         \n"
		"	color = vec4(1.0, 0.8, 0.0, 1.0);      \n"
		"}                                         \n"
	};

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);

#ifdef SE_DEBUG_ON
	char log[2048];
	int length = 0;
	glGetShaderInfoLog(vertexShader, 2048, &length, log);
	std::cout << log << std::endl;
#endif

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glBindAttribLocation(program, 1, "mypos");
	glLinkProgram(program);
	
#ifdef SE_DEBUG_ON
	int a = glGetAttribLocation(program, "mypos");
	std::cout << a << std::endl;
#endif	

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void DrawPixel(GLfloat x, GLfloat y, GLfloat size)
{
	GLuint program;
	GLuint vaos[1] = {0};
	GLuint bufs[1] = {0};
	glGenVertexArrays(1, vaos);
	glBindVertexArray(vaos[0]);

	GLfloat vertices[2] = {x, y};

	glGenBuffers(1, bufs);
	glBindBuffer(GL_ARRAY_BUFFER, bufs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	program = CompileShaders();
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glUseProgram(program);
	glPointSize(size);
	glBindVertexArray(vaos[0]);
	glDrawArrays(GL_POINTS, 0, 1);
}



