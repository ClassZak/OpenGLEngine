#pragma once
#include "AVertex.hpp"
#include <glm/glm.hpp>


class Vertex3DNormText : public AVertex<float>
{
public:
	glm::vec3 m_pos = {0.f, 0.f, 0.f};
	glm::vec3 m_normal = { 0.f, 0.f, 0.f };
	glm::vec2 m_texturePos = {0.f,0.f};


	Vertex3DNormText() = default;

	Vertex3DNormText(const glm::vec3& pos) :
	m_pos(pos) {}
	Vertex3DNormText(const glm::vec3& pos, const glm::vec2& texturePos) :
	m_pos(pos), m_texturePos(texturePos) {}
	Vertex3DNormText(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& texturePos) :
	m_pos(pos), m_texturePos(texturePos), m_normal(normal) {}

	Vertex3DNormText(const glm::vec2& pos)
	{
		m_pos.x = pos.x;
		m_pos.y = pos.y;
	}
	Vertex3DNormText(const glm::vec2& pos, const glm::vec2& texturePos) :
	m_texturePos(texturePos)
	{
		m_pos.x = pos.x;
		m_pos.y = pos.y;
	}


	Vertex3DNormText(const Vertex3DNormText&) = default;
	Vertex3DNormText& operator=(const Vertex3DNormText&) = default;

	bool operator==(const Vertex3DNormText& other) const noexcept
	{
		if(this == &other)
			return true;

		return m_pos == other.m_pos && m_texturePos == other.m_texturePos && m_normal == other.m_normal;
	}
	bool operator!=(const Vertex3DNormText& other) const noexcept
	{
		if (this == &other)
			return false;

		return m_pos != other.m_pos || m_texturePos != other.m_texturePos || m_normal != other.m_normal;
	}


	Vertex3DNormText operator+(const Vertex3DNormText& other)
	{
		Vertex3DNormText result(m_pos + other.m_pos, m_normal + other.m_normal, m_texturePos + other.m_texturePos);

		return result;
	}
	Vertex3DNormText operator-(const Vertex3DNormText& other)
	{
		Vertex3DNormText result(m_pos - other.m_pos, m_normal + other.m_normal, m_texturePos - other.m_texturePos);

		return result;
	}


	// ������������� ������� ��� ������
	friend std::ostream& operator<<(std::ostream& os, const Vertex3DNormText& obj)
	{
		os	<< '(' << obj.m_pos.x		<< ",\t" <<	obj.m_pos.y			<< ",\t" << obj.m_pos.z		<< ")\t"
			<< '(' << obj.m_normal.x	<< ",\t" <<	obj.m_normal.y		<< ",\t" << obj.m_normal.z	<< ")\t"
			<<		obj.m_texturePos.x	<< ",\t" << obj.m_texturePos.y;
		return os;
	}

	// ������������� ������� ��� �����
	friend std::istream& operator>>(std::istream& is, Vertex3DNormText& obj)
	{
		is	>> obj.m_pos.x			>> obj.m_pos.y			>> obj.m_pos.z 
			>> obj.m_normal.x		>> obj.m_normal.y		>> obj.m_normal.z
			>> obj.m_texturePos.x	>> obj.m_texturePos.y;
		return is;
	}
};

