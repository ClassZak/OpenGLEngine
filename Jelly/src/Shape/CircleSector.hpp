#pragma once
#define _USE_MATH_DEFINES

#include "AShape.hpp"
#include "../OpenGLClass/Shader.hpp"
#include "../Shape/Interfaces/IHasIndexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexArrayObject.hpp"
#include "../Shape/Interfaces/IHasShader.hpp"
#include "../Shape/Interfaces/IDrawable.hpp"
#include "../Shape/Interfaces/IHasVertexVector.hpp"

#include "../Vertex/Vertex2D.hpp"


template <typename T>
class CircleSector :
	public virtual IHasVertexArrayObject,
	public virtual IHasVertexBufferObject,
	public virtual IHasIndexBufferObject,
	public virtual IHasShader,
	public virtual IHasVertexVector<Vertex2D<T>>
{
protected:
	T m_radius;
	const Vertex2D<T>& m_center;
	double m_startDegree, m_endDegree;
public:
	CircleSector
	(
		std::size_t count,
		T radius,
		const Vertex2D<T>& center,
		double startDegree,
		double endDegree
	) : m_radius(radius), m_center(center), m_startDegree(startDegree), m_endDegree(endDegree)
	{
		std::vector<Vertex2D<T>> vertices=
		GenerateCircleSectorVertexes(count, radius, center, startDegree, endDegree);
		vertices.insert(vertices.begin(), center);
		
		::IHasVertexVector<Vertex2D<T>>::Init(vertices);
		Init(vertices, count);
	}

	CircleSector(const CircleSector<T>& other) :
	IHasVertexVector<Vertex2D<T>>(other), 
	m_radius(other.m_radius), 
	m_center(other.m_center), 
	m_startDegree(other.m_startDegree), 
	m_endDegree(other.m_endDegree)
	{
		Init(other.m_vertices, other.m_vertices.size() - 1);
	}
	

protected:
	void Init(const std::vector<Vertex2D<T>>& vertices, std::size_t count)
	{
		VertexBufferObject* newVertexBufferObject =
		new VertexBufferObject(vertices);
		this->m_vertexBufferObject.reset(newVertexBufferObject);

		// Attribute crafting
		VertexBufferLayout layout;
		layout.Push<T>(2);
		this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);

		IndexBufferObject* newIndexBufferObject = new IndexBufferObject(GenerateCircleSectorVertexIndexes(count));
		this->m_indexBufferObject.reset(newIndexBufferObject);

		this->m_vertexArrayObject.UnBind();
		this->m_vertexBufferObject.get()->UnBind();
		this->m_indexBufferObject.get()->UnBind();
	}
public:

	/// <summary>
	/// Генерация вершин для отрисовки окружности по сектору
	/// </summary>
	/// <param name="count">Число вершин (больше 1)</param>
	/// <param name="radius">Радиус (больше 0)</param>
	/// <param name="center">Центр</param>
	/// <param name="startDegree">Радианы для начала э [-2PI;2PI]</param>
	/// <param name="endDegree">Радианы для конца э [-2PI;2PI]</param>
	/// <returns>Вершины для отрисовки</returns>
	static std::vector<Vertex2D<T>> GenerateCircleSectorVertexes
	(
		std::size_t circlePointCount,
		T radius,
		const Vertex2D<T>& center,
		double startDegree,
		double endDegree
	)
	{
		if (circlePointCount <= 1u)
			throw std::invalid_argument("Division by zero");

		if (radius < 0)
			throw std::invalid_argument("Negative radius");

		if ((startDegree < -2 * M_PI or endDegree < -2 * M_PI) ||
			(startDegree > 2 * M_PI or endDegree > 2 * M_PI))
			throw std::invalid_argument("Degree out of range");

		if (startDegree > endDegree)
			std::swap(startDegree, endDegree);


		std::vector<Vertex2D<T>> vertexes;
		vertexes.reserve(circlePointCount);
		T sector = (endDegree - startDegree) / (circlePointCount - 1);

		for (std::size_t i = 1; i <= circlePointCount; ++i)
		{
			T	x = static_cast<T>(cos((startDegree + sector) * (i - 1)) * radius),
				y = static_cast<T>(sin((startDegree + sector) * (i - 1)) * radius);
			vertexes.emplace_back(x + center.x, y + center.y);
		}

		return vertexes;
	}

	/// <summary>
	/// Генерация индексов вершин для отрисовки окружности по сектору
	/// </summary>
	/// <param name="count">Число вершин (больше 1)</param>
	/// <param name="radius">Радиус (больше 0)</param>
	/// <param name="center">Центр</param>
	/// <param name="startDegree">Радианы для начала э [-2PI;2PI]</param>
	/// <param name="endDegree">Радианы для конца э [-2PI;2PI]</param>
	/// <returns>Вершины для отрисовки</returns>
	static std::vector<unsigned int> GenerateCircleSectorVertexIndexes(unsigned int OldCirclePointCount)
	{
		if (OldCirclePointCount == 0u)
			throw std::invalid_argument("Division by zero exception");

		std::vector<unsigned int> vertexesIndices;
		for (int i = 0; i < OldCirclePointCount - 1; i++)
		{
			vertexesIndices.push_back(0); // Центральная вершина
			vertexesIndices.push_back(1 + i); // Текущая вершина окружности
			vertexesIndices.push_back(1 + (i + 1) % OldCirclePointCount); // Следующая вершина окружности
		}

		return vertexesIndices;
	}
};