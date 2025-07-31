#pragma once
#include <memory>

#include "../../OpenGLClass/IndexBufferObject.hpp"
#include "IDrawable.hpp"

class IHasIndexBufferObject : public IDrawableOpenGL
{
protected:
	std::shared_ptr<IndexBufferObject> m_indexBufferObject = nullptr;
public:
	IHasIndexBufferObject() = default;
	virtual ~IHasIndexBufferObject() = default;


	std::shared_ptr<IndexBufferObject> GetIndexBufferObjectSharedPointer()
	{
		return m_indexBufferObject;
	}
	IndexBufferObject* GetIndexBufferObject()
	{
		return m_indexBufferObject.get();
	}


	void SetIndexBufferObject(IndexBufferObject indexBufferObject)
	{
		m_indexBufferObject.reset(&indexBufferObject);
	}
	void SetIndexBufferObject(const std::initializer_list<GLuint>& dataVector)
	{
		SetIndexBufferObject(IndexBufferObject(dataVector));
	}
	void SetIndexBufferObject(const std::vector<GLuint>& dataVector)
	{
		SetIndexBufferObject(IndexBufferObject(dataVector));
	}
	void SetIndexBufferObject(const GLuint* data, GLsizeiptr size)
	{
		SetIndexBufferObject(IndexBufferObject(data, size));
	}
};