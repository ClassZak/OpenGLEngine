#pragma once
#include "../../OpenGLClass/IndexBufferObject.hpp"
#include "IDrawable.hpp"

class IHasIndexBufferObject : IDrawableOpenGL
{
protected:
	IndexBufferObject m_indexBufferObject;
public:
	IHasIndexBufferObject() = default;
	virtual ~IHasIndexBufferObject() = default;


	IndexBufferObject& GetIndexBufferObject()
	{
		return m_indexBufferObject;
	}
};