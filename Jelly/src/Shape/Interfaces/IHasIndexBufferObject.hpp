#pragma once
#include "../../OpenGLClass/IndexBufferObject.hpp"

class IHasShader
{
protected:
	IndexBufferObject m_indexBufferObject;
public:
	IHasShader() = default;
	virtual ~IHasShader() = default;


	IndexBufferObject& GetIndexBufferObject()
	{
		return m_indexBufferObject;
	}
};