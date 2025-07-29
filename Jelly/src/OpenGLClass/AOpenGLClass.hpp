#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class AOpenGLClass
{
protected:
	GLuint m_index = 0;
public:
	GLuint GetId() const
	{
		return m_index;
	}
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
};