#pragma once
#include <memory>

#include "../../OpenGLClass/IndexBufferObject.hpp"
#include "IDrawable.hpp"

class IHasIndexBufferObject : virtual public IDrawableOpenGL
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


	void SetIndexBufferObject(IndexBufferObject* indexBufferObject)
	{
		m_indexBufferObject.reset(indexBufferObject);
	}
};