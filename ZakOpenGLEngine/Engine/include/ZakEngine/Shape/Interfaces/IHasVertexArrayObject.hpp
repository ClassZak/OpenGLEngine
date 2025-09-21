#pragma once
#include "../../OpenGLClass/VertexArrayObject.hpp"
#include "IDrawable.hpp"

namespace Zak {
class IHasVertexArrayObject : virtual public IDrawableOpenGL
{
protected:
	VertexArrayObject m_vertexArrayObject;
public:
	IHasVertexArrayObject() = default;
	virtual ~IHasVertexArrayObject() = default;


	VertexArrayObject& GetVertexArrayObject()
	{
		return m_vertexArrayObject;
	}
};
}