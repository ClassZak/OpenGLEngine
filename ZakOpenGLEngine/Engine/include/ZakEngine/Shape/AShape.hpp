#pragma once
#include "../Vertex/AVertex.hpp"
#include "../utils/GLMacro.h"
#include "Interfaces/IDrawable.hpp"

#include <vector>
#include <functional>

#define VERTEX_ATTRIBUTE_SIZE 2

namespace Zak {
/// <summary>
/// Класс для элементарных фигур
/// </summary>
/// <typeparam name="TVertex">Тип вершины, наследник AVertex<T></typeparam>
template<typename TVertex>
class AShape : public IDrawable
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

	AShape() = default;

	virtual ~AShape()
	{
		if(this->m_VAO)
			glDeleteVertexArrays(1, &this->m_VAO);
		if(this->m_VBO)
			glDeleteBuffers(1, &this->m_VBO);
		if(this->m_IBO)
			glDeleteBuffers(1, &this->m_IBO);
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
	const std::function<void(void)>&  GetShaderUniformsProgram() const
	{
		return m_shaderUniformsProgram;
	}


	AShape(const AShape<TVertex>& other)
	: m_shaderUniformsProgram(other.m_shaderUniformsProgram),
	m_vertexes(other.m_vertexes),
	m_indexes(other.m_indexes),
	m_VAO(0),
	m_VBO(0),
	m_IBO(0)
	{
	}
};
}