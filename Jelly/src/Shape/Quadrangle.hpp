#pragma once 
#define _USE_MATH_DEFINES

#include "../OpenGLClass/Shader.hpp"
#include "../Shape/Interfaces/IHasIndexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexArrayObject.hpp"
#include "../Shape/Interfaces/IHasShader.hpp"
#include "../Shape/Interfaces/IDrawable.hpp"
#include "../Vertex/Vertex2D.hpp"

#include <functional>
#include <stdexcept>
#include <cmath>


class Quadrangle : 
	public IHasVertexArrayObject,
	public IHasVertexBufferObject,
	public IHasIndexBufferObject,
	public IHasShader
{
public:
	template<typename T>
	Quadrangle
	(
		std::initializer_list<Vertex2D<T>> vertices
	) : Quadrangle(std::vector<Vertex2D<T>>(vertices))
	{
	}
	template<typename T>
	Quadrangle
	(
		const std::vector<Vertex2D<T>>& vertices
	)
	{
		if(vertices.size()!=4u)
			throw std::invalid_argument("Wrong size of vector");

		VertexBufferObject* newVertexBufferObject = new VertexBufferObject(vertices);
		this->m_vertexBufferObject.reset(newVertexBufferObject);

		// Arribute crafting
		VertexBufferLayout layout;
		layout.Push<T>(2);
		this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);

		IndexBufferObject* newIndexBufferObject = new IndexBufferObject({ 0, 1, 2, 0, 2, 3});
		this->m_indexBufferObject.reset(newIndexBufferObject);
	}
};