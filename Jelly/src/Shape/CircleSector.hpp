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



class CircleSector :
	public IHasVertexArrayObject,
	public IHasVertexBufferObject,
	public IHasIndexBufferObject,
	public IHasShader
{
public:
	template<typename T>
	CircleSector
	(
		std::size_t count,
		T radius,
		const Vertex2D<T>& center,
		double startDegree,
		double endDegree
	)
	{
		std::vector<Vertex2D<T>> vertices=
		GenerateCircleSectorVertexes(count, radius, center, startDegree, endDegree);
		vertices.insert(vertices.begin(), center);
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

	/// <summary>
	/// ��������� ������ ��� ��������� ���������� �� �������
	/// </summary>
	/// <param name="count">����� ������ (������ 1)</param>
	/// <param name="radius">������ (������ 0)</param>
	/// <param name="center">�����</param>
	/// <param name="startDegree">������� ��� ������ � [-2PI;2PI]</param>
	/// <param name="endDegree">������� ��� ����� � [-2PI;2PI]</param>
	/// <returns>������� ��� ���������</returns>
	template<typename T>
	static std::vector<Vertex2D<T>> GenerateCircleSectorVertexes
	(
		std::size_t OldCirclePointCount,
		T radius,
		const Vertex2D<T>& center,
		double startDegree,
		double endDegree
	)
	{
		if (OldCirclePointCount <= 1u)
			throw std::invalid_argument("Division by zero");

		if (radius < 0)
			throw std::invalid_argument("Negative radius");

		if ((startDegree < -2 * M_PI or endDegree < -2 * M_PI) ||
			(startDegree > 2 * M_PI or endDegree > 2 * M_PI))
			throw std::invalid_argument("Degree out of range");

		if (startDegree > endDegree)
			std::swap(startDegree, endDegree);


		std::vector<Vertex2D<T>> vertexes;
		vertexes.reserve(OldCirclePointCount);
		T sector = (endDegree - startDegree) / (OldCirclePointCount - 1);

		for (std::size_t i = 1; i <= OldCirclePointCount; ++i)
		{
			T	x = static_cast<T>(cos((startDegree + sector) * (i - 1)) * radius),
				y = static_cast<T>(sin((startDegree + sector) * (i - 1)) * radius);
			vertexes.emplace_back(x + center.x, y + center.y);
		}

		return vertexes;
	}

	/// <summary>
	/// ��������� �������� ������ ��� ��������� ���������� �� �������
	/// </summary>
	/// <param name="count">����� ������ (������ 1)</param>
	/// <param name="radius">������ (������ 0)</param>
	/// <param name="center">�����</param>
	/// <param name="startDegree">������� ��� ������ � [-2PI;2PI]</param>
	/// <param name="endDegree">������� ��� ����� � [-2PI;2PI]</param>
	/// <returns>������� ��� ���������</returns>
	static std::vector<unsigned int> GenerateCircleSectorVertexIndexes(unsigned int OldCirclePointCount)
	{
		if (OldCirclePointCount == 0u)
			throw std::invalid_argument("Division by zero exception");

		std::vector<unsigned int> vertexesIndices;
		for (int i = 0; i < OldCirclePointCount - 1; i++)
		{
			vertexesIndices.push_back(0); // ����������� �������
			vertexesIndices.push_back(1 + i); // ������� ������� ����������
			vertexesIndices.push_back(1 + (i + 1) % OldCirclePointCount); // ��������� ������� ����������
		}

		return vertexesIndices;
	}
};