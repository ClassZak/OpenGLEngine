#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#ifndef _WIN32
#include <csignal>
#endif


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



#if defined(_WIN32)
	#define GL_BREAK() __debugbreak()
#elif defined(__linux__) || defined(__APPLE__)
	#define GL_BREAK() std::raise(SIGTRAP) // Linux/macOS
#else
	#define GL_BREAK() // Пусто для других ОС
#endif
#define GL_ASSERT(x) do { \
	if (!(x)) { \
		GL_BREAK(); \
	} \
} while (0)
#define GLLogCall(x) GLClearError();\
	x;\
	GL_ASSERT(GLLogCallFunction());

