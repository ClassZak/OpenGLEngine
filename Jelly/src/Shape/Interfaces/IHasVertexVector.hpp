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

	/// <summary>
	/// Перепривязка данных
	/// </summary>
	/// <param name="startIndex">Индекс первого элемента</param>
	/// <param name="count">Количество перерисовываемых элементов</param>
	inline void ReBind(size_t startIndex, size_t count)
	{
		this->m_vertexArrayObject.Bind();
		this->m_vertexBufferObject->Bind();
		this->m_vertexBufferObject->ReBind
		(
			m_vertices.data() + startIndex,
			count * sizeof(Vertex2D<T>),
			startIndex * sizeof(Vertex2D<T>)
		);
		this->m_vertexArrayObject.UnBind();
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


	virtual void ReBind()
	{
		ReBind(0, m_vertices.size());
	}
};