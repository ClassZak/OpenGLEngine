#pragma once
#include "IDrawable.hpp"
#include "Line.hpp"

#include <vector>
#include <list>


class Jelly : public IDrawable
{
protected:
	GLuint m_shaderProgram=0;
	std::list<Line<float>> m_lines;
	std::list<Line<float>> m_createdLines;
public:
	Jelly();
	Jelly(GLuint shaderProgram) : Jelly()
	{
		m_shaderProgram=shaderProgram;
	}

	void Init() override;
	void Draw() override;

	/// <summary>
	/// Метод для дёрганья желе
	/// </summary>
	/// <param name="xCoefficient">значение от 0 до 1</param>
	/// <param name="yCoefficient">значение от 0 до 1</param>
	void Tugging(float xCoefficient, float yCoefficient);
};





