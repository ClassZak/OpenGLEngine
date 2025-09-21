#pragma once
#include "../OpenGLClass/Shader.hpp"
#include "../Shape/Interfaces/IHasIndexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexArrayObject.hpp"
#include "../Shape/Interfaces/IHasShader.hpp"
#include "../Shape/Interfaces/IDrawable.hpp"

#include "../Shape/Interfaces/IHasVertexVector.hpp"
#include "../Vertex/Vertex2D.hpp"

namespace Zak {
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
	T m_width, m_height;
public:
	SimpleRect(const Vertex2D<T>& pos, T width, T height) :
	m_pos(pos), m_width(width), m_height(height)
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




	inline Vertex2D<T> GetPos() const
	{
		return m_pos;
	}
	inline void SetPos(const Vertex2D<T>& pos)
	{
		m_pos = pos;
		::IHasVertexVector<T>::ReBind();
	}





	inline void SetSizes(const Vertex2D<T>& sizes)
	{
		this->m_vertices[1].y = m_pos.y + sizes.y;
		this->m_vertices[2].x = m_pos.x + sizes.x;
		this->m_vertices[2].y = m_pos.y + sizes.y;
		this->m_vertices[3].x = m_pos.x + sizes.x;
		::IHasVertexVector<T>::ReBind(1, 3);

		m_width = sizes.x;
		m_height = sizes.y;
	}
	inline void SetSizes(T width, T height)
	{
		SetSizes(Vertex2D<T>((T)width, (T)height));

		m_width = width, m_height = height;
	}

	inline void SetWidth(const T width)
	{
		this->m_vertices[2].x = m_pos.x + width;
		this->m_vertices[3].x = m_pos.x + width;
		::IHasVertexVector<T>::ReBind(2, 2);

		m_width = width;
	}
	inline void SetHeight(const T height)
	{
		this->m_vertices[1].y = m_pos.y + height;
		this->m_vertices[2].y = m_pos.y + height;
		::IHasVertexVector<T>::ReBind(1, 2);

		m_height = height;
	}




	inline std::pair<T, T> GetSizes() const
	{
		return std::pair<T,T>(m_width, m_height);
	}
	inline T GetWidth() const
	{
		return m_width;
	}
	inline T GetHeight() const 
	{
		return m_height;
	}




	SimpleRect(const SimpleRect<T>& other) : 
	SimpleRect(other.m_pos, other.m_width, other.m_height){}
};
}