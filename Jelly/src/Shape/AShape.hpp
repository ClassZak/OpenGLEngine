#pragma once
#include <vector>

#include "../Vertex/AVertex.hpp"
#include "../utils/GLMacro.h"
#include "IDrawable.hpp"

#define VERTEX_ATTRIBUTE_SIZE 2

/// <summary>
/// Класс для элементарных фигур
/// </summary>
/// <typeparam name="TVertex">Тип вершины, наследник AVertex<T></typeparam>
template<typename TVertex>
class AShape : IDrawable
{
protected:
	std::function<void(void)> m_shaderUniformsProgram;
	std::vector<TVertex> m_vertexes;
	std::vector<unsigned int> m_indexes;

	unsigned int m_VAO{};
	unsigned int m_VBO{};
	unsigned int m_IBO{};

	virtual void Init() = 0;
	virtual void Draw() = 0;

	virtual ~AShape()
	{
		if(this->m_VAO)
			glDeleteVertexArrays(1, &this->m_VAO);
		if(this->m_VBO)
			glDeleteBuffers(1, &this->m_VBO);
	}

public:
	std::vector<TVertex>& GetVertexes()
	{
		return m_vertexes;
	}
	std::vector<unsigned int>& GetIndexes()
	{
		return m_indexes;
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
};
