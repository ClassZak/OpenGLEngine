#pragma once 
#define _USE_MATH_DEFINES

#include <functional>
#include <cmath>

#include "AShape.hpp"
#include "../Vertex/Vertex2D.hpp"


template<class T>
class Circle : public AShape<Vertex2D<T>>
{
public:
	Circle
	(
		std::size_t count, 
		T radius, 
		const Vertex2D<T>& center, 
		unsigned int VAO = 0u,
		unsigned int VBO = 0u,
		unsigned int IBO = 0u
	)
	{
		this->m_vertexes = GenerateCircleVertexes(count, radius, center);
		this->m_indexes = GenerateCircleVertexIndexes(count);
		
		this->m_VAO = VAO;
		this->m_VBO = VBO;
		this->m_IBO = IBO;
	}
	Circle
	(
		std::size_t count, 
		T radius, 
		const Vertex2D<T>& center, 
		const std::function<void(void)>& shaderUniformsProgram,
		unsigned int VAO = 0u,
		unsigned int VBO = 0u,
		unsigned int IBO = 0u
	) : Circle(count, radius, center, VAO, VBO, IBO)
	{
		this->SetShaderUniformsProgram(shaderUniformsProgram);
	}

	~Circle()
	{
		glDeleteVertexArrays(1, &this->m_VAO);
		glDeleteBuffers(1, &this->m_VBO);
	}

	void Init() override;
	void Draw() override;

public:
	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T">скалярный тип</typeparam>
	/// <param name="count"></param>
	/// <param name="center"></param>
	/// <returns></returns>
	static std::vector<Vertex2D<T>> GenerateCircleVertexes
	(std::size_t count, T radius, const Vertex2D<T>& center)
	{
		std::vector<Vertex2D<T>> vertexes;
		vertexes.reserve(count + 1);

		vertexes.emplace_back(center);

		T sector = 2 * M_PI / (count-1);

		for (std::size_t i = 1; i <= count; ++i)
		{
			T x = static_cast<T>(cos(sector * (i-1)) * radius), y = static_cast<T>(sin(sector * (i-1)) * radius);
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
		// Генерация индексов для треугольников
		std::vector<unsigned int> vertexesIndices;
		for (int i = 0; i < circlePointCount - 1; i++)
		{
			vertexesIndices.push_back(0); // Центральная вершина
			vertexesIndices.push_back(1 + i); // Текущая вершина окружности
			vertexesIndices.push_back(1 + (i + 1) % circlePointCount); // Следующая вершина окружности
		}

		return vertexesIndices;
	}
};



template<class T>
inline void Circle<T>::Draw()
{
	GLLogCall(glBindVertexArray(this->m_VAO));
	GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO));

	GLLogCall(glBufferData
	(
		GL_ARRAY_BUFFER,
		this->m_vertexes.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
		this->m_vertexes.data(),
		GL_STATIC_DRAW
	));
	GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IBO));

	if (this->m_shaderUniformsProgram)
		this->m_shaderUniformsProgram();

	GLLogCall(glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		this->m_indexes.size() * sizeof(std::vector<unsigned int>::value_type),
		this->m_indexes.data(),
		GL_STATIC_DRAW
	));

	GLLogCall(glDrawElements(GL_TRIANGLES, this->m_indexes.size(), GL_UNSIGNED_INT, nullptr));
}
template<class T>
inline void Circle<T>::Init()
{
	if (!this->m_VAO)
		GLLogCall(glGenVertexArrays(1, &this->m_VAO));
	GLLogCall(glBindVertexArray(this->m_VAO));

	if (!this->m_VBO)
		GLLogCall(glGenBuffers(1, &this->m_VBO));
	GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO));

	glBufferData
	(
		GL_ARRAY_BUFFER,
		this->m_vertexes.size() * sizeof(Vertex2D<T>),
		this->m_vertexes.data(),
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer
	(0, VERTEX_ATTRIBUTE_SIZE, GL_FLOAT, GL_FALSE, sizeof(T) * VERTEX_ATTRIBUTE_SIZE, 0);

	if (!this->m_IBO)
		glGenBuffers(1, &this->m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IBO);

	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		this->m_indexes.size() * sizeof(unsigned int),
		this->m_indexes.data(),
		GL_STATIC_DRAW
	);
}
