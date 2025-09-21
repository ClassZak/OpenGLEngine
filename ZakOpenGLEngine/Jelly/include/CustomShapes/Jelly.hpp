#pragma once
#define _USE_MATH_DEFINES
#include <ZakEngine/OpenGLClass/VertexArrayObject.hpp>
#include <ZakEngine/OpenGLClass/VertexBufferObject.hpp>
#include <ZakEngine/OpenGLClass/VertexBufferLayout.hpp>
#include <ZakEngine/OpenGLClass/IndexBufferObject.hpp>
#include <ZakEngine/OpenGLClass/Shader.hpp>

#include <ZakEngine/EngineCore/Renderer.hpp>

#include <ZakEngine/Shape/Interfaces/IHasIndexBufferObject.hpp>
#include <ZakEngine/Shape/Interfaces/IHasVertexBufferObject.hpp>
#include <ZakEngine/Shape/Interfaces/IHasVertexArrayObject.hpp>
#include <ZakEngine/Shape/Interfaces/IHasShader.hpp>
#include <ZakEngine/Shape/Interfaces/IDrawable.hpp>

#include <ZakEngine/Shape/Line.hpp>
#include <ZakEngine/Shape/Circle.hpp>
#include <ZakEngine/Shape/Quadrangle.hpp>
#include <ZakEngine/Shape/CircleSector.hpp>

#include <vector>
#include <list>

class Jelly
{
protected:

	std::list<Zak::Line<float>> m_createdLines;
	std::list<Zak::Line<float>> m_lines;
	std::list<Zak::Line<float>> m_createdRoundedLines;
	std::list<Zak::Line<float>> m_roundedLines;

	std::list<Zak::Quadrangle<float>> m_createdQuadrangles;
	std::list<Zak::Quadrangle<float>> m_quadrangles;
	std::list<Zak::CircleSector<float>> m_createdCircleSectors;
	std::list<Zak::CircleSector<float>> m_circleSectors;


	const float START_X = -0.75f;
	const float END_X = 0.75f;

	const float START_Y = -0.75f;
	const float END_Y = 0.5f;

	const float LENGTH = abs(START_X - END_X);

	const unsigned int PARTS_COUNT = 5u;
	const float MULTIPLE_COEFFICIENT = 0.9f;

	const float PART_SIZE = LENGTH / PARTS_COUNT;


	const float ROUNDED_LINES_VERTEX_COUNT = 20;

	Zak::Line<float>* m_bottomLine= 
	new Zak::Line<float>({ Zak::Vertex2D<float>(START_X, START_Y), Zak::Vertex2D<float>(END_X, START_Y)});



	inline void AnimateLines(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);
	inline void AnimateRoundedLines(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);
	inline void AnimateParts(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);
	inline void AnimateRoundedParts(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);
public:
	Jelly()=default;

	void Init(const std::string& shaderName = "default_shader");
	void Draw();

	/// <summary>
	/// Метод для дёрганья желе по времени
	/// </summary>
	/// <param name="millisecondsSinceEpoch">время</param>
	void Animate(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);
};