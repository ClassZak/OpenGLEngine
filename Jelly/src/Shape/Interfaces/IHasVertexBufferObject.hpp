#pragma once
#include "../../OpenGLClass/VertexBufferObject.hpp"
#include "IDrawable.hpp"

class IHasVertexBufferObject : IDrawableOpenGL
{
protected:
	VertexBufferObject m_vertexBufferObject;
public:
	IHasVertexBufferObject() = default;
	virtual ~IHasVertexBufferObject() = default;


	VertexBufferObject& GetVertexBufferObject()
	{
		return m_vertexBufferObject;
	}
};