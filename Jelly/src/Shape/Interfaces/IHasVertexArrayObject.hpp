#pragma once
#include "../../OpenGLClass/VertexArrayObject.hpp"
#include "IDrawable.hpp"

class IHasVertexArrayObject : public IDrawableOpenGL
{
protected:
	VertexArrayObject m_vertexBufferObject;
public:
	IHasVertexArrayObject() = default;
	virtual ~IHasVertexArrayObject() = default;


	VertexArrayObject& GetVertexArrayObject()
	{
		return m_vertexBufferObject;
	}
};