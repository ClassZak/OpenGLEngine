#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <functional>

#include "../utils/GLMacro.h"
#include "AOpenGLClass.hpp"

class VertexBufferObject : public AOpenGLClass
{
	GLsizei m_size;
	GLsizei m_count;
	GLenum m_usage;
public:
	template<class T>
	VertexBufferObject(const std::vector<T>& dataVector, GLenum usage = GL_DYNAMIC_DRAW)
	: VertexBufferObject(dataVector.data(), dataVector.size() * sizeof(T), dataVector.size(), usage)
	{ }
	VertexBufferObject(const void* data, GLsizeiptr size, GLsizei count, GLenum usage = GL_DYNAMIC_DRAW) :
	m_usage(usage)
	{
		Init(data, size, count);
	}

	~VertexBufferObject()
	{
		glDeleteBuffers(1, &this->m_index);
	}

	template<class T>
	void Init(const std::vector<T>& dataVector)
	{
		Init(dataVector.data(), dataVector.size() * sizeof(T), dataVector.size());
	}
	void Init(const void* data, GLsizeiptr size, GLsizei count, GLenum usage = GL_DYNAMIC_DRAW)
	{
		m_size = size;
		m_count = count;
		GLLogCall(glGenBuffers(1, &this->m_index));
		GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_index));
		GLLogCall(
		glBufferData
		(
			GL_ARRAY_BUFFER,
			size,
			data,
			m_usage
		));
	}


	void Bind() const override
	{
		GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_index));
	}

	void UnBind() const override
	{
		GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}


	template<class T>
	void ReBind(const std::vector<T>& dataVector,std::size_t count, GLintptr offset = 0)
	{
		if(count > dataVector.size())
			throw std::invalid_argument("Count of vertices grater than size of vector");

		Bind();
		ReBind(dataVector.data(), count * sizeof(T), offset);
		UnBind();
	}
	template<class T>
	void ReBind(const std::vector<T>& dataVector, GLintptr offset = 0)
	{
		Bind();
		ReBind(dataVector.data(), dataVector.size() * sizeof(T) - offset, offset);
		UnBind();
	}
	void ReBind(const void* data, const GLsizeiptr size, GLintptr offset = 0)
	{
		GLLogCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
	}


	GLsizei GetSize() const
	{
		return m_size;
	}
	GLsizei GetCount() const
	{
		return m_count;
	}
};


