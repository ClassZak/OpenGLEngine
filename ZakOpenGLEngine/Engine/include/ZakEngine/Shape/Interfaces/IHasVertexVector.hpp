#pragma once
#include <vector>

#include "IHasVertexArrayObject.hpp"
#include "IHasVertexBufferObject.hpp"
#include "../../Vertex/Vertex2D.hpp"
#include "../../Vertex/Vertex3DNormText.hpp"

namespace Zak {
template <class Vertex>
class IHasVertexVector : 
	virtual public IHasVertexBufferObject,
	virtual public IHasVertexArrayObject
{
protected:
	std::vector<Vertex> m_vertices;

	IHasVertexVector() = default;
	void Init(const std::initializer_list<Vertex>& vertices)
	{
		Init(std::vector<Vertex>(vertices));
	}
	void Init(const std::vector<Vertex>& vertices)
	{
		m_vertices.assign(vertices.begin(), vertices.end());
	}

	/// <summary>
	/// Привязка данных заново
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
			count * sizeof(Vertex),
			startIndex * sizeof(Vertex)
		);
		this->m_vertexArrayObject.UnBind();
	}
public:
	IHasVertexVector(const std::initializer_list<Vertex>& vertices) :
	IHasVertexVector(std::vector<Vertex>(vertices))
	{}
	IHasVertexVector(const std::vector<Vertex>& vertices)
	{
		Init(vertices);
	}

	std::vector<Vertex>& GetVertices()
	{
		return m_vertices;
	}


	virtual void ReBind()
	{
		ReBind(0, m_vertices.size());
	}



	IHasVertexVector(const IHasVertexVector<Vertex>& other) :
	IHasVertexVector(other.m_vertices) {}
	IHasVertexVector& operator=(const IHasVertexVector<Vertex>& other)
	{
		if(this!=&other)
			Init(other.m_vertices);
	}
};
}