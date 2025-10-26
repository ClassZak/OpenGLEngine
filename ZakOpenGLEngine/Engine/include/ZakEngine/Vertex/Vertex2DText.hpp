#pragma once

#include "AVertex.hpp"
#include "../utils/GLMHash.hpp"

namespace Zak {
class Vertex2DText : AVertex<float>
{
public:
	glm::vec2 m_pos = { 0.f, 0.f };
	glm::vec2 m_texturePos = { 0.f,0.f };

	Vertex2DText(const glm::vec2& pos) :
	m_pos(pos) {}
	Vertex2DText(const glm::vec2& pos, const glm::vec2& texturePos) :
	m_pos(pos), m_texturePos(texturePos) {}


	Vertex2DText(const Vertex2DText&) = default;
	Vertex2DText& operator=(const Vertex2DText&) = default;

	bool operator==(const Vertex2DText& other) const
	{
		if (this == &other)
			return true;

		return m_pos == other.m_pos && m_texturePos == other.m_texturePos;
	}
	bool operator!=(const Vertex2DText& other) const
	{
		if (this == &other)
			return false;

		return m_pos != other.m_pos || m_texturePos != other.m_texturePos;
	}


	Vertex2DText operator+(const Vertex2DText& other)
	{
		Vertex2DText result(m_pos + other.m_pos, m_texturePos + other.m_texturePos);

		return result;
	}
	Vertex2DText operator-(const Vertex2DText& other)
	{
		Vertex2DText result(m_pos - other.m_pos, m_texturePos - other.m_texturePos);

		return result;
	}


	// Дружественная функция для вывода
	friend std::ostream& operator<<(std::ostream& os, const Vertex2DText& obj)
	{
		os	<< '(' << obj.m_pos.x << ",\t" << obj.m_pos.y << ")\t" 
			<< obj.m_texturePos.x << ",\t" << obj.m_texturePos.y;
		return os;
	}

	// Дружественная функция для ввода
	friend std::istream& operator>>(std::istream& is, Vertex2DText& obj)
	{
		is >> obj.m_pos.x >> obj.m_pos.y >> obj.m_texturePos.x >> obj.m_texturePos.y;
		return is;
	}
};
}


// Хэш-функция для Vertex2DText
namespace std
{
	template<> struct hash<Zak::Vertex2DText>
	{
		size_t operator()(const Zak::Vertex2DText& vertex) const noexcept
		{
			size_t h1 = hash<glm::vec2>()(vertex.m_pos);
			size_t h2 = hash<glm::vec2>()(vertex.m_texturePos);

			return size_t(h1 ^ (h2 << 1));
		}
	};
}
