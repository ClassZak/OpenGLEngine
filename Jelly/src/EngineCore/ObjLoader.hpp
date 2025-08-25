#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>

#include "../Vertex/Vertex3DNormText.hpp"

// Хэш-функция для Vertex3DNormText для использования в unordered_map
namespace std
{
	// Специализации хэша для GLM типов
	template<> struct hash<glm::vec3>
	{
		size_t operator()(const glm::vec3& vec) const noexcept
		{
			size_t h1 = hash<float>()(vec.x);
			size_t h2 = hash<float>()(vec.y);
			size_t h3 = hash<float>()(vec.z);
			return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
		}
	};
	template<> struct hash<glm::vec2>
	{
		size_t operator()(const glm::vec2& vec) const noexcept
		{
			size_t h1 = hash<float>()(vec.x);
			size_t h2 = hash<float>()(vec.y);
			return h1 ^ (h2 << 1);
		}
	};


	template<> struct hash<Vertex3DNormText>
	{
		size_t operator()(const Vertex3DNormText& vertex) const noexcept
		{
			size_t h1 = hash<glm::vec3>()(vertex.m_pos);
			size_t h2 = hash<glm::vec3>()(vertex.m_normal);
			size_t h3 = hash<glm::vec2>()(vertex.m_texturePos);
			return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
		}
	};
}


class ObjLoader
{
public:
	static bool LoadMesh
	(
		const std::string& path,
		std::vector<Vertex3DNormText>& vertices,
		std::vector<unsigned int>& indices
	);
};