#pragma once
#include <vector>

#include "IHasVertexArrayObject.hpp"
#include "IHasVertexBufferObject.hpp"
#include "../../Vertex/Vertex2D.hpp"

template <typename T>
class IHasVertexVector : 
	virtual public IHasVertexBufferObject,
	virtual public IHasVertexArrayObject
{
protected:
	std::vector<Vertex2D<T>> m_vertices;

	IHasVertexVector() = default;
	void Init(const std::initializer_list<Vertex2D<T>>& vertices)
	{
		Init(std::vector<Vertex2D<T>>(vertices));
	}
	void Init(const std::vector<Vertex2D<T>>& vertices)
	{
		m_vertices.assign(vertices.begin(), vertices.end());
	}
public:
	IHasVertexVector(const std::initializer_list<Vertex2D<T>>& vertices) :
	IHasVertexVector(std::vector<Vertex2D<T>>(vertices))
	{}
	IHasVertexVector(const std::vector<Vertex2D<T>>& vertices)
	{
		Init(vertices);
	}

	std::vector<Vertex2D<T>>& GetVertices()
	{
		return m_vertices;
	}

	void ReBind(std::size_t count, GLintptr offset)
	{
		this->m_vertexArrayObject.Bind();
		this->m_vertexBufferObject.get()->ReBind(m_vertices, count, offset);
		this->m_vertexArrayObject.UnBind();
	}
	void ReBind(GLintptr offset = 0)
	{
		this->m_vertexArrayObject.Bind();
		this->m_vertexBufferObject.get()->ReBind(m_vertices, offset);
		this->m_vertexArrayObject.UnBind();
	}
};