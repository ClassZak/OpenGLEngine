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
			size_t h1 = hash<float>()(vertex.m_pos.x);
			size_t h2 = hash<float>()(vertex.m_pos.y);
			size_t h3 = hash<float>()(vertex.m_pos.z);
			size_t h4 = hash<float>()(vertex.m_normal.x);
			size_t h5 = hash<float>()(vertex.m_normal.y);
			size_t h6 = hash<float>()(vertex.m_normal.z);
			size_t h7 = hash<float>()(vertex.m_texturePos.x);
			size_t h8 = hash<float>()(vertex.m_texturePos.y);

			return ((((((h1 ^ (h2 << 1)) ^ (h3 << 2)) ^ (h4 << 3)) ^
				(h5 << 4)) ^ (h6 << 5)) ^ (h7 << 6)) ^ (h8 << 7);
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
		std::vector<unsigned int>& indices, 
		bool saveUnique = false
	);
};