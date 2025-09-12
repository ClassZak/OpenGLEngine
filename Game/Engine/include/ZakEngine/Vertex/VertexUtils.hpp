#pragma once
#include <algorithm>
#include <vector>
#include "AVertex.hpp"


template<class T>
static std::vector<T> GetUniqueVertexes(const std::vector<T>& vertexes)
{
	std::vector<T> uniqeVertexes;
	for (auto& vertex : vertexes)
		if (std::find(uniqeVertexes.begin(), uniqeVertexes.end(), vertex) == uniqeVertexes.end())
			uniqeVertexes.emplace_back(vertex);

	return uniqeVertexes;
}
template<class T>
static void SaveUniqueVertexes(std::vector<T>& vertexes)
{
	vertexes = GetUniqueVertexes(vertexes);
}