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
public:
	template<class T>
	VertexBufferObject(const std::vector<T>& dataVector)
	: VertexBufferObject(dataVector.data(), dataVector.size() * sizeof(T))
	{ }
	VertexBufferObject(const void* data, GLsizeiptr size)
	{
		Init(data, size);
	}

	~VertexBufferObject()
	{
		glDeleteBuffers(1, &this->m_index);
	}

	template<class T>
	void Init(const std::vector<T>& dataVector)
	{
		Init(dataVector.data(), dataVector.size() * sizeof(T));
	}
	void Init(const void* data, GLsizeiptr size)
	{
		GLLogCall(glGenBuffers(1, &this->m_index));
		GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_index));
		glBufferData
		(
			GL_ARRAY_BUFFER,
			size,
			data,
			GL_STATIC_DRAW
		);
	}


	void Bind() const override
	{
		GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_index));
	}
	
	void UnBind() const override
	{
		GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
};


