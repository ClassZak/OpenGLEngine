#pragma once 
#define _USE_MATH_DEFINES

#include "../OpenGLClass/Shader.hpp"
#include "../Shape/Interfaces/IHasIndexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexArrayObject.hpp"
#include "../Shape/Interfaces/IHasShader.hpp"
#include "../Shape/Interfaces/IDrawable.hpp"

#include "../Shape/Interfaces/IHasVertexVector.hpp"
#include "../Vertex/Vertex2D.hpp"
#include "../Vertex/Vertex2DText.hpp"

#include <functional>
#include <stdexcept>
#include <array>
#include <cmath>

namespace Zak {
template <typename T>
class Quadrangle : 
	public IHasVertexVector<Vertex2D<T>>,
	public IHasIndexBufferObject,
	public IHasShader
{
protected:
	void Init(const std::vector<Vertex2D<T>>& vertices)
	{
		if(vertices.size()!=4u)
			throw std::invalid_argument("Wrong size of vector");

		::Zak::IHasVertexVector<Vertex2D<T>>::Init(vertices);

		VertexBufferObject* newVertexBufferObject = new VertexBufferObject(vertices);
		this->m_vertexBufferObject.reset(newVertexBufferObject);

		// Attribute crafting
		VertexBufferLayout layout;
		layout.Push<T>(2);
		this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);

		IndexBufferObject* newIndexBufferObject = new IndexBufferObject({ 0, 1, 2, 0, 2, 3});
		this->m_indexBufferObject.reset(newIndexBufferObject);
	}
public:
	Quadrangle(std::initializer_list<Vertex2D<T>> vertices) :
	Quadrangle(std::vector<Vertex2D<T>>(vertices))
	{
	}
	Quadrangle(const std::array<Vertex2D<T>, 4u>& vertices) : 
	Quadrangle({vertices[0], vertices[1], vertices[2], vertices[3]})
	{
	}

	Quadrangle(const std::vector<Vertex2D<T>>& vertices)
	{
		Init(vertices);
	}

	Quadrangle(const Quadrangle<T>& other) :
	IHasVertexVector<Vertex2D<T>>(other)
	{
		Init(other.m_vertices);
	}
};




template <class Vertex2DClass>
class QuadrangleTemplate :
	public IHasVertexVector<Vertex2DClass>,
	public IHasIndexBufferObject,
	public IHasShader
{
protected:
	void Init(const std::vector<Vertex2DClass>& vertices)
	{
		if (vertices.size() != 4u)
			throw std::invalid_argument("Wrong size of vector");

		::Zak::IHasVertexVector<Vertex2DClass>::Init(vertices);

		VertexBufferObject* newVertexBufferObject = new VertexBufferObject(vertices);
		this->m_vertexBufferObject.reset(newVertexBufferObject);

		// Attribute crafting
		VertexBufferLayout layout;
		CraftAttributes<Vertex2DClass>(layout);
		this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);

		IndexBufferObject* newIndexBufferObject = new IndexBufferObject({ 0, 1, 2, 0, 2, 3 });
		this->m_indexBufferObject.reset(newIndexBufferObject);
	}
public:
	QuadrangleTemplate(std::initializer_list<Vertex2DClass> vertices) :
		QuadrangleTemplate(std::vector<Vertex2DClass>(vertices))
	{
	}
	QuadrangleTemplate(const std::array<Vertex2DClass, 4u>& vertices) :
		QuadrangleTemplate({ vertices[0], vertices[1], vertices[2], vertices[3] })
	{
	}

	QuadrangleTemplate(const std::vector<Vertex2DClass>& vertices)
	{
		Init(vertices);
	}

	QuadrangleTemplate(const QuadrangleTemplate<Vertex2DClass>& other) :
		IHasVertexVector<Vertex2DClass>(other)
	{
		Init(other.m_vertices);
	}



	// Attribute crafting
	template<typename Vertex2DClass>
	static void CraftAttributes(VertexBufferLayout& layout)
	{
#ifdef _WIN32
		static_assert(false);
#endif
	}
	template<>
	static void CraftAttributes<Vertex2D_float>(VertexBufferLayout& layout)
	{
		layout.Push<float>(2);
	}
	template<>
	static void CraftAttributes<Vertex2DText>(VertexBufferLayout& layout)
	{
		layout.Push<float>(2);
		layout.Push<float>(2);
	}
};
}









#include "../Shape/Interfaces/IHasTexture.hpp"
#include "../Shape/Interfaces/IHasVertexArrayObject.hpp"

namespace Zak {
template <class Vertex2DClass>
class QuadrangleTexture :
	virtual public IHasVertexVector<Vertex2DClass>,
	virtual public IHasTexture,
	virtual public IHasShader,
	virtual public IHasVertexArrayObject,
	virtual public IHasVertexBufferObject,
	virtual public IHasIndexBufferObject
{
protected:
	void Init(const std::vector<Vertex2DClass>& vertices)
	{
		if (vertices.size() != 4u)
			throw std::invalid_argument("Wrong size of vector");

		::Zak::IHasVertexVector<Vertex2DClass>::Init(vertices);

		VertexBufferObject* newVertexBufferObject = new VertexBufferObject(vertices);
		this->m_vertexBufferObject.reset(newVertexBufferObject);

		// Attribute crafting
		VertexBufferLayout layout;
		CraftAttributes<Vertex2DClass>(layout);
		this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);

		IndexBufferObject* newIndexBufferObject = new IndexBufferObject({ 0, 1, 2, 0, 2, 3 });
		this->m_indexBufferObject.reset(newIndexBufferObject);
	}
public:
	QuadrangleTexture(std::initializer_list<Vertex2DClass> vertices) :
		QuadrangleTexture(std::vector<Vertex2DClass>(vertices))
	{
	}
	QuadrangleTexture(const std::array<Vertex2DClass, 4u>& vertices) :
		QuadrangleTexture({ vertices[0], vertices[1], vertices[2], vertices[3] })
	{
	}

	QuadrangleTexture(const std::vector<Vertex2DClass>& vertices)
	{
		Init(vertices);
	}

	QuadrangleTexture(const QuadrangleTexture<Vertex2DClass>& other) :
		IHasVertexVector<Vertex2DClass>(other)
	{
		Init(other.m_vertices);
	}



	// Attribute crafting
	template<typename Vertex2DClass>
	static void CraftAttributes(VertexBufferLayout& layout)
	{
#ifdef _WIN32
		static_assert(false);
#endif
	}
	template<>
	static void CraftAttributes<Vertex2DText>(VertexBufferLayout& layout)
	{
		layout.Push<float>(2);
		layout.Push<float>(2);
	}
};
}
