#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <functional>

#include "../utils/GLMacro.h"

class IndexBufferObject
{
private:
	// Renderer id
	GLuint m_index = 0;
	GLsizeiptr count = 0;

	IndexBufferObject(const std::vector<GLuint>& dataVector)
		: IndexBufferObject(dataVector.data(), dataVector.size() * sizeof(GLuint))
	{
	}
	IndexBufferObject(const GLuint* data, GLsizeiptr size)
	{
		Init(data, size);
	}

	~IndexBufferObject()
	{
		glDeleteBuffers(1, &m_index);
	}
public:
	void Init(const std::vector<GLuint>& dataVector)
	{
		Init(dataVector.data(), dataVector.size());
	}

	void Init(const GLuint* data, GLsizeiptr count)
	{
		GLLogCall(glGenBuffers(1, &this->m_index));
		GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_index));
		glBufferData
		(
			GL_ELEMENT_ARRAY_BUFFER,
			count * sizeof(GLuint),
			data,
			GL_STATIC_DRAW
		);
	}


	void Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_index);
	}

	void UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};




