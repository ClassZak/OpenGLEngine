#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <functional>

#include "../utils/GLMacro.h"
#include "AOpenGLClass.hpp"

namespace Zak {
class IndexBufferObject : public AOpenGLClass
{
private:
	GLsizeiptr m_count = 0;

public:
	IndexBufferObject(const std::initializer_list<GLuint>& dataVector, GLenum usage = GL_STATIC_DRAW)
		: IndexBufferObject(std::vector<GLuint>(dataVector), usage)
	{}
	IndexBufferObject(const std::vector<GLuint>& dataVector, GLenum usage = GL_STATIC_DRAW)
		: IndexBufferObject(dataVector.data(), dataVector.size())
	{}
	IndexBufferObject(const GLuint* data, GLsizeiptr count, GLenum usage = GL_STATIC_DRAW)
	{
		Init(data, count, usage);
	}

	~IndexBufferObject()
	{
		glDeleteBuffers(1, &this->m_index);
	}

	void Init(const std::vector<GLuint>& dataVector, GLenum usage = GL_STATIC_DRAW)
	{
		Init(dataVector.data(), dataVector.size(), usage);
	}

	void Init(const GLuint* data, GLsizeiptr count, GLenum usage = GL_STATIC_DRAW)
	{
		m_count = count;
		GLLogCall(glGenBuffers(1, &this->m_index));
		GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_index));
		glBufferData
		(
			GL_ELEMENT_ARRAY_BUFFER,
			count * sizeof(GLuint),
			data,
			usage
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




	static inline std::vector<GLuint> GenerateIndecies(GLsizeiptr count)
	{
		std::vector<GLuint> result(count);

		for(GLsizeiptr i=0; i!=count; ++i)
			result[i]=i;

		return result;
	}
};
}