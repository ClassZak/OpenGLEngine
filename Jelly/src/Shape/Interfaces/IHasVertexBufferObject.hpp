#pragma once
#include <memory>

#include "../../OpenGLClass/VertexBufferObject.hpp"
#include "IDrawable.hpp"

class IHasVertexBufferObject : virtual public IDrawableOpenGL
{
protected:
	std::shared_ptr<VertexBufferObject> m_vertexBufferObject = nullptr;
public:
	IHasVertexBufferObject() = default;
	virtual ~IHasVertexBufferObject() = default;


	std::shared_ptr<VertexBufferObject> GetVertexBufferObjectSharedPointer()
	{
		return m_vertexBufferObject;
	}
	VertexBufferObject* GetVertexBufferObject()
	{
		return m_vertexBufferObject.get();
	}


	void SetVertexBufferObject(VertexBufferObject vertexBufferObject)
	{
		m_vertexBufferObject.reset(&vertexBufferObject);
	}
	template<class T>
	void SetVertexBufferObject(const std::vector<T>& dataVector)
	{
		SetVertexBufferObject(VertexBufferObject(dataVector));
	}
	void SetVertexBufferObject(const void* data, GLsizeiptr size)
	{
		SetVertexBufferObject(VertexBufferObject(data, size));
	}
};