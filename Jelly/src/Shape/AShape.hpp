#pragma once
#include <vector>

#include "../Vertex/AVertex.hpp"
#include "../utils/GLMacro.h"


template<class T>
class AShape
{
protected:
	std::vector<T> m_vertexes;
	std::vector<unsigned int> m_indexes;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
};
