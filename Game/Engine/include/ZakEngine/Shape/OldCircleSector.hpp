#pragma once
#define _USE_MATH_DEFINES

#include "AShape.hpp"
#include "Interfaces/IDrawable.hpp"
#include "../Vertex/Vertex2D.hpp"

namespace Zak {
template<typename T>
class OldCircleSector : public AShape<Vertex2D<T>>
{
public:
	OldCircleSector
	(
		std::size_t count,
		T radius,
		const Vertex2D<T>& center,
		double startDegree,
		double endDegree,
		const std::function<void(void)>& shaderUniformsProgram,
		unsigned int VAO = 0u,
		unsigned int VBO = 0u,
		unsigned int IBO = 0u
	) : OldCircleSector(count, radius, center, startDegree, endDegree, VAO, VBO, IBO)
	{
		AShape<Vertex2D<T>>::SetShaderUniformsProgram(shaderUniformsProgram);
	}

	OldCircleSector
	(
		std::size_t count,
		T radius,
		const Vertex2D<T>& center,
		double startDegree,
		double endDegree,
		unsigned int VAO = 0u,
		unsigned int VBO = 0u,
		unsigned int IBO = 0u
	)
	{
		this->m_vertexes = GenerateOldCircleSectorVertexes(count, radius, center, startDegree, endDegree);
		this->m_vertexes.insert(this->m_vertexes.begin(), center);
		this->m_indexes = GenerateOldCircleSectorVertexIndexes(count);


		this->m_VAO = VAO;
		this->m_VBO = VBO;
		this->m_IBO = IBO;
	}


	~OldCircleSector() = default;

	void Init() override;
	void Draw() override;

	/// <summary>
	/// Генерация вершин для отрисовки окружности по сектору
	/// </summary>
	/// <param name="count">Число вершин (больше 1)</param>
	/// <param name="radius">Радиус (больше 0)</param>
	/// <param name="center">Центр</param>
	/// <param name="startDegree">Радианы для начала э [-2PI;2PI]</param>
	/// <param name="endDegree">Радианы для конца э [-2PI;2PI]</param>
	/// <returns>Вершины для отрисовки</returns>
	static std::vector<Vertex2D<T>> GenerateOldCircleSectorVertexes
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
	/// Генерация индексов вершин для отрисовки окружности по сектору
	/// </summary>
	/// <param name="count">Число вершин (больше 1)</param>
	/// <param name="radius">Радиус (больше 0)</param>
	/// <param name="center">Центр</param>
	/// <param name="startDegree">Радианы для начала э [-2PI;2PI]</param>
	/// <param name="endDegree">Радианы для конца э [-2PI;2PI]</param>
	/// <returns>Вершины для отрисовки</returns>
	static std::vector<unsigned int> GenerateOldCircleSectorVertexIndexes(unsigned int OldCirclePointCount)
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



template<class T>
inline void OldCircleSector<T>::Init()
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
template<class T>
inline void OldCircleSector<T>::Draw()
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
}