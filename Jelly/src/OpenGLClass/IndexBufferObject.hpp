#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <functional>

#include "../utils/GLMacro.h"
#include "AOpenGLClass.hpp"

class IndexBufferObject : public AOpenGLClass
{
private:
	GLsizeiptr m_count = 0;

public:
	IndexBufferObject(const std::initializer_list<GLuint>& dataVector)
		: IndexBufferObject(std::vector<GLuint>(dataVector))
	{}
	IndexBufferObject(const std::vector<GLuint>& dataVector)
		: IndexBufferObject(dataVector.data(), dataVector.size())
	{}
	IndexBufferObject(const GLuint* data, GLsizeiptr count)
	{
		Init(data, count);
	}

	~IndexBufferObject()
	{
		glDeleteBuffers(1, &this->m_index);
	}

	void Init(const std::vector<GLuint>& dataVector)
	{
		Init(dataVector.data(), dataVector.size());
	}

	void Init(const GLuint* data, GLsizeiptr count)
	{
		m_count = count;
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


	void Bind() const override
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_index);
	}

	void UnBind() const override
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	GLsizeiptr GetCount() const
	{
		return m_count;
	}
};




