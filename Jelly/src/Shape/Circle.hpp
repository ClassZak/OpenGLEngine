#pragma once 
#include <functional>

#include "AShape.hpp"
#include "../Vertex/2DVertex.hpp"


template<class T>
class Circle : public AShape<T>
{
protected:
	std::function<void(void)> m_shaderUniformsProgram;

public:
	std::vector<T>& GetVertexes()
	{
		return m_vertexes;
	}
	std::vector<unsigned int>& GetIndexes()
	{
		return m_indexes;
	}


	unsigned int GetVAO() const
	{
		return m_VAO;
	}
	unsigned int GetVBO() const
	{
		return m_VBO;
	}
	unsigned int GetIBO() const
	{
		return m_IBO;
	}

	void SetVAO(unsigned int VAO)
	{
		m_VAO = VAO;
	}
	void SetVBO(unsigned int VBO)
	{
		m_VBO = VBO;
	}
	void SetIBO(unsigned int IBO)
	{
		m_IBO = IBO;
	}


	void SetShaderUniformsProgram(const std::function<void(void)>& shaderUniformsProgram)
	{
		m_shaderUniformsProgram = shaderUniformsProgram;
	}




	Circle
	(
		std::size_t count, 
		T radius, 
		const Vertex2D<T>& center, 
		unsigned int VAO, 
		unsigned int VBO, 
		unsigned int IBO
	)
	{
		m_vertexes = GenerateCircleVertexes(count, radius, center);
		m_indexes = GenerateCircleVertexIndexes(count);
		
		m_VAO = VAO;
		m_VBO = VBO;
		m_IBO = IBO;
	}
	Circle
	(
		std::size_t count, 
		T radius, 
		const Vertex2D<T>& center, 
		unsigned int VAO, 
		unsigned int VBO, 
		unsigned int IBO,
		const std::function<void(void)>& shaderUniformsProgram
	) : Circle(count, radius, center, VAO, VBO, IBO)
	{
		SetShaderUniformsProgram(shaderUniformsProgram);
	}

	void Bind()
	{
		GLLogCall(glGenVertexArrays(1, &m_VAO));
		GLLogCall(glBindVertexArray(m_VAO));

		GLLogCall(glGenBuffers(1, &m_VBO));
		GLLogCall(glBindBuffer(GL_ARRAY_BUFFER,m_VBO));

		glBufferData
		(
			GL_ARRAY_BUFFER,
			m_vertexes.size() * sizeof(Vertex2D<T>),
			m_vertexes.data(),
			GL_STATIC_DRAW
		);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer
		(0, VERTEX_ATTRIBUTE_SIZE, GL_FLOAT, GL_FALSE, sizeof(T) * VERTEX_ATTRIBUTE_SIZE, 0);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

		glBufferData
		(
			GL_ELEMENT_ARRAY_BUFFER,
			m_indexes.size() * sizeof(unsigned int),
			m_indexes.data(),
			GL_STATIC_DRAW
		);
	}

	void Draw()
	{
		glBindVertexArray(m_VAO)
		glBindBuffer(GL_ARRAY_BUFFER,m_VBO);

		glBufferData
		(
			GL_ARRAY_BUFFER, 
			m_vertexes.size() * sizeof(std::vector<Vertex2D<T>>::value_type),
			m_vertexes.data(), 
			GL_STATIC_DRAW
		)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		m_shaderUniformsProgram();

		glBufferData
		(
			GL_ELEMENT_ARRAY_BUFFER, 
			m_indexes.size() * sizeof(std::vector<unsigned int>::value_type) * m_vertexes.size(),
			m_indexes.data(),
			GL_STATIC_DRAW
		);

		glDrawElements(GL_TRIANGLES, m_vertexes.size(), GL_UNSIGNED_INT, nullptr);
	}


	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T">скалярный тип</typeparam>
	/// <param name="count"></param>
	/// <param name="center"></param>
	/// <returns></returns>
	template<typename T>
	static std::vector<Vertex2D<T>> GenerateCircleVertexes
	(std::size_t count, T radius, const Vertex2D<T>& center)
	{
		std::vector<Vertex2D<T>> vertexes;
		vertexes.reserve(count + 1);

		vertexes.emplace_back(center);

		double sector = 2 * M_PI / count;

		for (std::size_t i = 0; i < count; ++i)
		{
			T x = static_cast<T>(cos(sector * i) * radius), y = static_cast<T>(sin(sector * i) * radius);
			vertexes.emplace_back(x + center.x, y + center.y);
		}

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

