#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

static inline void GLClearError()
{
	while(glGetError() != GL_NO_ERROR);
}

static inline bool GLLogCallFunction()
{
	while (GLenum error = glGetError())
	{
		printf("[OpenGL error] (%d)\n", error);
		return false;
	}
	return true;
}

#define GL_ASSERT(x) if(!(x)) __debugbreak();
#define GLLogCall(x) GLClearError();\
	x;\
	GL_ASSERT(GLLogCallFunction());

