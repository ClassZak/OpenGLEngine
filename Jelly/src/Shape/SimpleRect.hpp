#pragma once
#include "../OpenGLClass/Shader.hpp"
#include "../Shape/Interfaces/IHasIndexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexArrayObject.hpp"
#include "../Shape/Interfaces/IHasShader.hpp"
#include "../Shape/Interfaces/IDrawable.hpp"

#include "../Shape/Interfaces/IHasVertexVector.hpp"
#include "../Vertex/Vertex2D.hpp"


template<typename T>
class SimpleRect : 
	public virtual IHasVertexArrayObject, 
	public virtual IHasVertexBufferObject,
	public virtual IHasIndexBufferObject,
	public virtual IHasShader,
	protected virtual IHasVertexVector<T>
{
protected:
	Vertex2D<T> m_pos;
public:
	SimpleRect(const Vertex2D<T>& pos, T width, T height) : m_pos(pos)
	{
		std::vector<Vertex2D<T>> vertices =
		{
			pos,
			Vertex2D(pos.x, pos.y + height),
			pos + Vertex2D(width, height),
			Vertex2D(pos.x + width, pos.y),
		};
		this->Init(vertices);

		VertexBufferObject* newVertexBufferObject = new VertexBufferObject(vertices);
		this->m_vertexBufferObject.reset(newVertexBufferObject);

		// Attribute crafting
		VertexBufferLayout layout;
		layout.Push<T>(2);
		this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);

		IndexBufferObject* newIndexBufferObject = new IndexBufferObject({ 0, 1, 2, 0, 2, 3 });
		this->m_indexBufferObject.reset(newIndexBufferObject);


		this->m_vertexArrayObject.UnBind();
		this->m_vertexBufferObject.get()->UnBind();
		this->m_indexBufferObject.get()->UnBind();
	}


	inline void SetSizes(const Vertex2D<T>& sizes)
	{
		this->m_vertices[1] = Vertex2D<T>(m_pos.x, m_pos.y + sizes.y);
		this->m_vertices[2] = m_pos + Vertex2D<T>(sizes.x, sizes.y);
		this->m_vertices[3] = Vertex2D<T>(m_pos.x + sizes.x, sizes.y);
		this->ReBind(1);
	}
	inline void SetWidth(const T width)
	{
		this->m_vertices[2].x = m_pos.x + width;
		this->m_vertices[3].x = m_pos.x + width;
		this->ReBind(2, sizeof(Vertex2D<T>) * 2);
	}
	inline void SetHeight(const T height)
	{
		this->m_vertices[1].y = m_pos.y + height;
		this->m_vertices[2].y = m_pos.y + height;
		this->ReBind(2, 0);
	}
};
