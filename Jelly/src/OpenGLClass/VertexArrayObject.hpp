#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <functional>

#include "../utils/GLMacro.h"
#include "VertexBufferObject.hpp"
#include "VertexBufferLayout.hpp"
#include "AOpenGLClass.hpp"

class VertexArrayObject : public AOpenGLClass
{
public:
	void AddBuffer
	(
		const VertexBufferObject& vertexBufferObject,
		const VertexBufferLayout& vertexBufferLayout
	)
	{
		this->Bind();
		vertexBufferObject.Bind();
		const auto& elements = vertexBufferLayout.GetElements();
		unsigned long long offset{0};
		for (std::size_t i = 0; i != elements.size(); ++i)
		{
			const auto& element = elements[i];
			GLLogCall(glEnableVertexAttribArray(i));

			switch (element.type)
			{
				case GL_FLOAT:
				{
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
					break;
				}
				case GL_UNSIGNED_INT:
				{
					GLLogCall
					(
						glVertexAttribIPointer
						(
							i,
							element.count,
							element.type,
							vertexBufferLayout.GetStride(),
							(const void*)offset
						)
					);
					break;
				}
			}
			
			
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


	void Bind() const override
	{
		GLLogCall(glBindVertexArray(this->m_index));
	}
	void UnBind() const override
	{
		GLLogCall(glBindVertexArray(0));
	}
};


