#pragma once
#define _USE_MATH_DEFINES
#include "Interfaces/IDrawable.hpp"
#include "OldLine.hpp"
#include "OldCircle.hpp"
#include "OldQuadrangle.hpp"
#include "OldCircleSector.hpp"

#include <vector>
#include <list>
#include <stdlib.h>


class Jelly : public IDrawable
{
protected:

	std::list<OldLine<float>> m_createdLines;
	std::list<OldLine<float>> m_lines;
	std::list<OldLine<float>> m_createdRoundedLines;
	std::list<OldLine<float>> m_roundedLines;

	std::list<OldQuadrangle<float>> m_createdOldQuadrangles;
	std::list<OldQuadrangle<float>> m_OldQuadrangles;
	std::list<OldCircleSector<float>> m_createdOldCircleSectors;
	std::list<OldCircleSector<float>> m_OldCircleSectors;


	const float START_X = -0.75f;
	const float END_X = 0.75f;

	const float START_Y = -0.75f;
	const float END_Y = 0.5f;

	const float LENGTH = abs(START_X - END_X);

	const unsigned int PARTS_COUNT = 5u;
	const float MULTIPLE_COEFFICIENT = 0.9f;

	const float PART_SIZE = LENGTH / PARTS_COUNT;


	const float ROUNDED_LINES_VERTEX_COUNT = 20;

	GLuint m_shaderProgram=0;
	OldLine<float>* m_bottomLine= new OldLine<float>({Vertex2D<float>(START_X, START_Y), Vertex2D<float>(END_X, START_Y)});



	inline void AnimateLines(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);
	inline void AnimateRoundedLines(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);
	inline void AnimateParts(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);
	inline void AnimateRoundedParts(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);
public:
	Jelly()=default;
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





