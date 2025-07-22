#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexBufferObject
{
private:
	GLuint m_index=0;

public:
	void Bind()
	{
		
	}
	void UnBind()
	{
		if (m_index != GL_FALSE)
		{
			glDeleteBuffers(1, &m_index);
			m_index = GL_FALSE;
		}
	}
};


