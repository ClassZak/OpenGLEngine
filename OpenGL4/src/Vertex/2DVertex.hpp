#pragma once
#include "AVertex.hpp"

template<typename T>
struct Vertex2D : AVertex<T>
{
	T x, y;

	T GetVector()
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}


	Vertex2D()
	{
		x = y = 0.f;
	}
	Vertex2D(T x, T y) : x(x), y(y)
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

