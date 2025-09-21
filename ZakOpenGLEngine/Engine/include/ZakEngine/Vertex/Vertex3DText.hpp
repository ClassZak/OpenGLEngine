#pragma once

#include "AVertex.hpp"
#include "../utils/GLMHash.hpp"

namespace Zak {
class Vertex3DText : AVertex<float>
{
	glm::vec3 m_pos = {0.f, 0.f, 0.f};
	glm::vec2 m_texturePos = {0.f,0.f};

public:
	Vertex3DText(const glm::vec3& pos) :
	m_pos(pos) {}
	Vertex3DText(const glm::vec3& pos, const glm::vec2& texturePos) :
	m_pos(pos), m_texturePos(texturePos) {}

	Vertex3DText(const glm::vec2& pos)
	{
		m_pos.x = pos.x;
		m_pos.y = pos.y;
		m_pos.z = 0;
	}
	Vertex3DText(const glm::vec2& pos, const glm::vec2& texturePos) :
	m_texturePos(texturePos)
	{
		m_pos.x = pos.x;
		m_pos.y = pos.y;
		m_pos.z = 0;
	}


	Vertex3DText(const Vertex3DText&) = default;
	Vertex3DText& operator=(const Vertex3DText&) = default;

	bool operator==(const Vertex3DText& other)
	{
		if(this == &other)
			return true;

		return m_pos == other.m_pos && m_texturePos == other.m_texturePos;
	}
	bool operator!=(const Vertex3DText& other)
	{
		if (this == &other)
			return false;

		return m_pos != other.m_pos || m_texturePos != other.m_texturePos;
	}


	Vertex3DText operator+(const Vertex3DText& other)
	{
		Vertex3DText result(m_pos + other.m_pos, m_texturePos + other.m_texturePos);

		return result;
	}
	Vertex3DText operator-(const Vertex3DText& other)
	{
		Vertex3DText result(m_pos - other.m_pos, m_texturePos - other.m_texturePos);

		return result;
	}


	// Дружественная функция для вывода
	friend std::ostream& operator<<(std::ostream& os, const Vertex3DText& obj)
	{
		os << '(' << obj.m_pos.x << ",\t" << obj.m_pos.y << ",\t" << obj.m_pos.z <<
			")\t"<< obj.m_texturePos.x << ",\t" << obj.m_texturePos.y;
		return os;
	}

	// Дружественная функция для ввода
	friend std::istream& operator>>(std::istream& is, Vertex3DText& obj)
	{
		is >> obj.m_pos.x >> obj.m_pos.y >> obj.m_pos.z >> obj.m_texturePos.x >> obj.m_texturePos.y;
		return is;
	}
};
}