#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <functional>

#include "../utils/GLMacro.h"
#include "VertexBufferObject.hpp"
#include "VertexBufferLayout.hpp"

class VertexArrayObject
{
private:
	// Renderer id
	GLuint m_index = 0;


public:
	void AddBuffer
	(
		const VertexBufferObject& vertexBufferObject,
		const VertexBufferLayout& vertexBufferLayout
	) const
	{
		this->Bind();
		vertexBufferObject.Bind();
		const auto& elements = vertexBufferLayout.GetElements();
		unsigned long long offset{0};
		for (std::size_t i = 0; i != elements.size(); ++i)
		{
			const auto& element = elements[i];
			GLLogCall(glEnableVertexAttribArray(i));
			GLLogCall
			(
				glVertexAttribPointer
				(
					i,
					element.count,
					element.type,
					element.is_normalized,
					vertexBufferLayout.GetStride(),
					(const void*)offset
				)
			);
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
	}
	VertexArrayObject()
	{
		glGenVertexArrays(1, &m_index);
	}
	~VertexArrayObject()
	{
		glDeleteVertexArrays(1, &m_index);
	}


	void Bind() const
	{
		GLLogCall(glBindVertexArray(this->m_index));
	}
	void UnBind() const
	{
		GLLogCall(glBindVertexArray(0));
	}

	GLuint GetId() const
	{
		return m_index;
	}
};


