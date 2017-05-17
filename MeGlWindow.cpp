#include <GL\glew.h>
#include <MeGlWindow.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <glm\glm.hpp>
#include <Vertex.h>

using namespace std;
const GLuint NUM_VERTICES_PER_TRI = 3;

void sendDataToOpenGL()
{
	Vertex myTri[] =
	{
		glm::vec3(+0.0f, +1.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),

		glm::vec3(-1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +0.0f),

		glm::vec3(+1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),

	};

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(myTri), myTri, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (char*)(sizeof(GL_FLOAT) * 3));

}

void MeGlWindow::paintGL()
{
	/*To set the color of the viewport*/
	//glClearColor(1, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width(), height());
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_PER_TRI);
}
bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetter,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType
	)
{
	GLint Status;
	
	objectPropertyGetter(objectID, statusType, &Status);

	if (Status != GL_TRUE)
	{
		GLint infoLogLength = 0;

		objectPropertyGetter(objectID, statusType, &infoLogLength);

		if (infoLogLength > 1)
		{
			GLchar* infoLogBuffer = (char*)malloc(sizeof(char) * infoLogLength);

			GLsizei bufferSize;
			getInfoLogFunc(objectID, infoLogLength, &bufferSize, infoLogBuffer);

			printf("Error in Shader:\n %s \n", infoLogBuffer);

			free(infoLogBuffer);

		}
		return false;
	}
	return true;
}


bool checkShaderStatus(GLuint shaderID)
{
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint infoLogLength = 0;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 1)
		{
			GLchar* infoLogBuffer = (char*)malloc(sizeof(char) * infoLogLength);

			GLsizei bufferSize;
			glGetShaderInfoLog(shaderID, infoLogLength, &bufferSize, infoLogBuffer);

			printf("Error Compiling Vertex/Fragment Shader:\n %s \n", infoLogBuffer);

			free(infoLogBuffer);

		}
		return false;
	}
	return true;
}

bool checkProgramStatus(GLuint programID)
{
	GLint compileStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint infoLogLength = 0;

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 1)
		{
			GLchar* infoLogBuffer = (char*)malloc(sizeof(char) * infoLogLength);

			GLsizei bufferSize;
			glGetProgramInfoLog(programID, infoLogLength, &bufferSize, infoLogBuffer);

			printf("Error linking program:\n %s \n", infoLogBuffer);

			free(infoLogBuffer);

		}
		return false;
	}
	return true;
}


string readShaderCode(const char *fileName)
{
	ifstream meInput(fileName);

	if (!meInput.good())
	{
		cout << "File Failed to Load ..." << fileName;
		exit(1);
	}

	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];

	//adapter[0] = vertexShaderCode;
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);

	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);

	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	GLuint programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	/*Bind the the Attribute location 0 with the vPosition name*/
	glBindAttribLocation(programID, 0, "vPosition");
	glBindAttribLocation(programID, 1, "vertexColor");

	glLinkProgram(programID);
	
	if (!checkProgramStatus(programID))
		return;

	glUseProgram(programID);

}

void MeGlWindow::initializeGL()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);

	printf("GL Shading language version %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	sendDataToOpenGL();
	installShaders();

}
