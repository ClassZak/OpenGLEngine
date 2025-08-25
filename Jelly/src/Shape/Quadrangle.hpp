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

#include <functional>
#include <stdexcept>
#include <array>
#include <cmath>


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

		::IHasVertexVector<Vertex2D<T>>::Init(vertices);

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