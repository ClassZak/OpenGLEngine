#pragma once
#define _USE_MATH_DEFINES
#include "../OpenGLClass/VertexArrayObject.hpp"
#include "../OpenGLClass/VertexBufferObject.hpp"
#include "../OpenGLClass/VertexBufferLayout.hpp"
#include "../OpenGLClass/IndexBufferObject.hpp"
#include "../OpenGLClass/Shader.hpp"

#include "../EngineCore/Renderer.hpp"

#include "Interfaces/IHasIndexBufferObject.hpp"
#include "Interfaces/IHasVertexBufferObject.hpp"
#include "Interfaces/IHasVertexArrayObject.hpp"
#include "Interfaces/IHasShader.hpp"
#include "Interfaces/IDrawable.hpp"

#include "Line.hpp"
#include "Circle.hpp"
#include "Quadrangle.hpp"
#include "CircleSector.hpp"

#include <vector>
#include <list>
#include <stdlib.h>

#include <gl/GL.h>


class Jelly
{
protected:

	std::list<Line<float>> m_createdLines;
	std::list<Line<float>> m_lines;
	std::list<Line<float>> m_createdRoundedLines;
	std::list<Line<float>> m_roundedLines;

	std::list<Quadrangle<float>> m_createdQuadrangles;
	std::list<Quadrangle<float>> m_quadrangles;
	std::list<CircleSector<float>> m_createdCircleSectors;
	std::list<CircleSector<float>> m_circleSectors;


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
	Line<float>* m_bottomLine= new Line<float>({Vertex2D<float>(START_X, START_Y), Vertex2D<float>(END_X, START_Y)});



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

	void Init();
	void Draw();

	/// <summary>
	/// Метод для дёрганья желе по времени
	/// </summary>
	/// <param name="millisecondsSinceEpoch">время</param>
	void Animate(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);
};





