#pragma once 
#define _USE_MATH_DEFINES

#include <functional>
#include <cmath>

#include "AShape.hpp"
#include "../Vertex/2DVertex.hpp"


template<class T>
class Circle : public AShape<Vertex2D<T>>
{
protected:
	std::function<void(void)> m_shaderUniformsProgram;

public:
	std::vector<Vertex2D<T>>& GetVertexes()
	{
		return this->m_vertexes;
	}
	std::vector<unsigned int>& GetIndexes()
	{
		return this->m_indexes;
	}


	unsigned int GetVAO() const
	{
		return this->m_VAO;
	}
	unsigned int GetVBO() const
	{
		return this->m_VBO;
	}
	unsigned int GetIBO() const
	{
		return this->m_IBO;
	}

	void SetVAO(unsigned int VAO)
	{
		this->m_VAO = VAO;
	}
	void SetVBO(unsigned int VBO)
	{
		this->m_VBO = VBO;
	}
	void SetIBO(unsigned int IBO)
	{
		this->m_IBO = IBO;
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
		SetShaderUniformsProgram(shaderUniformsProgram);
	}

	~Circle()
	{
		glDeleteVertexArrays(1, &this->m_VAO);
	}

	void Bind()
	{
		if(!this->m_VAO)
			GLLogCall(glGenVertexArrays(1, &this->m_VAO));
		GLLogCall(glBindVertexArray(this->m_VAO));

		if (!this->m_VBO)
			GLLogCall(glGenBuffers(1, &this->m_VBO));
		GLLogCall(glBindBuffer(GL_ARRAY_BUFFER,this->m_VBO));

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

	void Draw()
	{
		GLLogCall(glBindVertexArray(this->m_VAO));
		GLLogCall(glBindBuffer(GL_ARRAY_BUFFER,this->m_VBO));

		GLLogCall(glBufferData
		(
			GL_ARRAY_BUFFER, 
			this->m_vertexes.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
			this->m_vertexes.data(), 
			GL_STATIC_DRAW
		));
		GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IBO));

		if(m_shaderUniformsProgram)
			m_shaderUniformsProgram();

		GLLogCall(glBufferData
		(
			GL_ELEMENT_ARRAY_BUFFER, 
			this->m_indexes.size() * sizeof(std::vector<unsigned int>::value_type),
			this->m_indexes.data(),
			GL_STATIC_DRAW
		));

		GLLogCall(glDrawElements(GL_TRIANGLES, this->m_indexes.size(), GL_UNSIGNED_INT, nullptr));
	}


public:
	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T">��������� ���</typeparam>
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
	/// ��������� �������� ��� ���������
	/// </summary>
	/// <param name="circlePointCount">����� �����</param>
	/// <returns>������� ��� ���������</returns>
	static std::vector<unsigned int> GenerateCircleVertexIndexes(unsigned int circlePointCount)
	{
		// ��������� �������� ��� �������������
		std::vector<unsigned int> vertexesIndices;
		for (int i = 0; i < circlePointCount - 1; i++)
		{
			vertexesIndices.push_back(0); // ����������� �������
			vertexesIndices.push_back(1 + i); // ������� ������� ����������
			vertexesIndices.push_back(1 + (i + 1) % circlePointCount); // ��������� ������� ����������
		}

		return vertexesIndices;
	}
};

