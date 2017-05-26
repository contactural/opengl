#include <GL\glew.h>
#include <MeGlWindow.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <Vertex.h>
#include <ShapeGenerator.h>

using namespace std;
using glm::vec3;
using glm::mat4;

const GLuint NUM_VERTICES_PER_TRI = 3;
GLuint programID;
GLuint numIndices;

void sendDataToOpenGL()
{
	ShapeData shape = ShapeGenerator::makeCube();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (char*)(sizeof(GL_FLOAT) * 3));

	
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(),
		shape.indices, GL_STATIC_DRAW);

	numIndices = shape.numIndices;

	shape.cleanup();

}

void MeGlWindow::paintGL()
{
	/*To set the color of the viewport*/
	//glClearColor(1, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width(), height());
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// projection * translation * rotation * vertex

	GLint fullTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "fullTransformMatrix");

	mat4 fullTransformMatrix;
	mat4 projectionMatrix	= glm::perspective(60.0f, ((float)width() / height()), 0.1f, 10.0f);

	//Cube 1
	mat4 translationMatrix = glm::translate(vec3(-1.0f, 0.0f, -3.0f));
	mat4 rotationMatrix = glm::rotate(36.0f, vec3(1.0f, 0.0f, 0.0f));

	fullTransformMatrix = projectionMatrix * translationMatrix * rotationMatrix;

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1,
		GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	//Cube 2
	translationMatrix = glm::translate(vec3(1.0f, 0.0f, -3.75f));
	rotationMatrix = glm::rotate(126.0f, vec3(0.0f, 1.0f, 0.0f));

	fullTransformMatrix = projectionMatrix * translationMatrix * rotationMatrix;

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1,
		GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);


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

	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	/*Bind the the Attribute location 0 with the vPosition name*/
	glBindAttribLocation(programID, 0, "vPosition");
	glBindAttribLocation(programID, 1, "vertexColor");

	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

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

MeGlWindow::~MeGlWindow()
{
	glUseProgram(0);
	glDeleteProgram(programID);
}