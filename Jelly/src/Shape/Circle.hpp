#pragma once 
#define _USE_MATH_DEFINES

#include "AShape.hpp"
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
#include <GL/glew.h>


class Circle :
	public IHasVertexArrayObject,
	public IHasVertexBufferObject,
	public IHasIndexBufferObject,
	public IHasShader
{
public:
	template<typename T>
	Circle
	(
		std::size_t count,
		T radius,
		const Vertex2D<T>& center
	)
	{
		VertexBufferObject* newVertexBufferObject =
		new VertexBufferObject(GenerateCircleVertexes(count, radius, center));
		this->m_vertexBufferObject.reset(newVertexBufferObject);

		// Attribute crafting
		VertexBufferLayout layout;
		layout.Push<T>(2);
		this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);
		
		IndexBufferObject* newIndexBufferObject = new IndexBufferObject(GenerateCircleVertexIndexes(count));
		this->m_indexBufferObject.reset(newIndexBufferObject);
		
		this->m_vertexArrayObject.UnBind();
		this->m_vertexBufferObject.get()->UnBind();
		this->m_indexBufferObject.get()->UnBind();
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="count">Число вершин (больше 1)</param>
	/// <param name="radius">Радиус (больше 0)</param>
	/// <param name="center">Центр</param>
	/// <returns>Вершины для отрисовки</returns>
	template<typename T>
	static std::vector<Vertex2D<T>> GenerateCircleVertexes
	(std::size_t count, T radius, const Vertex2D<T>& center)
	{
		if (count <= 1u)
			throw std::invalid_argument("Division by zero");

		if (radius < 0)
			throw std::invalid_argument("Negative radius");


		std::vector<Vertex2D<T>> vertexes;
		vertexes.reserve(count + 1);

		vertexes.emplace_back(center);

		T sector = 2 * M_PI / (count - 1);

		for (std::size_t i = 1; i <= count; ++i)
		{
			T	x = static_cast<T>(cos(sector * (i - 1)) * radius),
				y = static_cast<T>(sin(sector * (i - 1)) * radius);
			vertexes.emplace_back(x + center.x, y + center.y);
		}

		vertexes.emplace_back(*std::next(vertexes.begin()));

		return vertexes;
	}

	/// <summary>
	/// Генерация индексов для отрисовки
	/// </summary>
	/// <param name="circlePointCount">Число точек</param>
	/// <returns>Индексы для отрисовки</returns>
	static std::vector<unsigned int> GenerateCircleVertexIndexes(unsigned int circlePointCount)
	{
		if (circlePointCount == 0u)
			throw std::invalid_argument("Division by zero exception");

		std::vector<unsigned int> vertexesIndices;
		for (int i = 0; i < circlePointCount - 1; i++)
		{
			vertexesIndices.push_back(0); // Центральная вершина
			vertexesIndices.push_back(1 + i); // Текущая вершина окружности
			vertexesIndices.push_back(1 + (i + 1) % circlePointCount); // Следующая вершина окружности
		}

		return vertexesIndices;
	}


	/// <summary>
	/// Генерация вершин для отрисовки окружности по сектору
	/// </summary>
	/// <param name="count">Число вершин (больше 1)</param>
	/// <param name="radius">Радиус (больше 0)</param>
	/// <param name="center">Центр</param>
	/// <param name="startDegree">Радианы для начала э [-2PI;2PI]</param>
	/// <param name="endDegree">Радианы для конца э [-2PI;2PI]</param>
	/// <returns>Вершины для отрисовки</returns>
	template<typename T>
	static std::vector<Vertex2D<T>> GenerateRoundVertexes
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
		T sector = (endDegree - startDegree) / (circlePointCount-1);

		for (std::size_t i = 1; i <= circlePointCount; ++i)
		{
			T	x = static_cast<T>(cos((startDegree + sector) * (i-1)) * radius),
				y = static_cast<T>(sin((startDegree + sector) * (i-1)) * radius);
			vertexes.emplace_back(x + center.x, y + center.y);
		}

		return vertexes;
	}
};