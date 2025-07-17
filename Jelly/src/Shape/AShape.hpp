#pragma once
#include <vector>

#include "../Vertex/AVertex.hpp"
#include "../utils/GLMacro.h"

#define VERTEX_ATTRIBUTE_SIZE 2

template<typename TVertex>
class AShape
{
protected:
	std::vector<TVertex> m_vertexes;
	std::vector<unsigned int> m_indexes;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
};
