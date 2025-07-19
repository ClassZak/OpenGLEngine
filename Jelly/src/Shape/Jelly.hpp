#pragma once
#include "IDrawable.hpp"
#include "Line.hpp"

#include <vector>
#include <list>
#include <stdlib.h>


class Jelly : public IDrawable
{
protected:
	GLuint m_shaderProgram=0;
	std::list<Line<float>> m_lines;
	std::list<Line<float>> m_createdLines;

	const float START_X = -0.75f;
	const float END_X = 0.75f;

	const float START_Y = -0.75f;
	const float END_Y = 0.5f;

	const float LENGTH = abs(START_X - END_X);

	const unsigned int PARTS_COUNT = 5u;
	const float MULTIPLE_COEFFICIENT = 0.7f;

	Line<float> m_bottomLine= Line<float>({Vertex2D<float>(START_X, START_Y), Vertex2D<float>(END_X, START_Y)});

public:
	Jelly();
	Jelly(GLuint shaderProgram) : Jelly()
	{
		m_shaderProgram=shaderProgram;
	}

	void Init() override;
	void Draw() override;

	/// <summary>
	/// Метод для дёрганья желе по времени
	/// </summary>
	/// <param name="millisecondsSinceEpoch">время</param>
	void Animate(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);
};





