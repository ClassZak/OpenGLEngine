#pragma once
#include <glm/glm.hpp>

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
}

