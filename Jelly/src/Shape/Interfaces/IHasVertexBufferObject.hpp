#pragma once
#include "../../OpenGLClass/VertexBufferObject.hpp"

class IHasVertexBufferObject
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