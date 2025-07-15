#pragma once
#include "AVertex.hpp"

struct Vertex2D : AVertex<float>
{
	float x, y;

	float GetVector()
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}


	Vertex2D()
	{
		x = y = 0.f;
	}
	Vertex2D(float x, float y) : x(x), y(y)
	{
	}

	Vertex2D(const Vertex2D&) = default;
	Vertex2D& operator=(const Vertex2D&) = default;

	bool operator==(const Vertex2D& other)
	{
		return this->x == other.x && this->y == other.y;
	}
	bool operator!=(const Vertex2D& other)
	{
		return this->x != other.x || this->y != other.y;
	}
};

