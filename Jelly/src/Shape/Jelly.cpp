#include "Jelly.hpp"

Jelly::Jelly()
{
}



void Jelly::Init()
{
	const float START_X	= -0.75f;
	const float END_X	= 0.75f;

	const float START_Y	= -0.75f;
	const float END_Y	= 0.5f;


	const float LENGTH = abs(START_X - END_X);

	const unsigned int PARTS_COUNT = 5u;
	const float MULTIPLE_COEFFICIENT = 0.7f;

	m_lines.push_back
	(Line<float>({Vertex2D(START_X,START_Y),Vertex2D(END_X,START_Y) }));


	for (int i = 0; i <= PARTS_COUNT; ++i)
	{
		float x= START_X + i * LENGTH / PARTS_COUNT;
		m_lines.push_back
		(Line<float>({ Vertex2D(x,START_Y),Vertex2D(x * MULTIPLE_COEFFICIENT,END_Y) }));
	}





	if (m_shaderProgram)
	{
		int location = glGetUniformLocation(m_shaderProgram, "u_Color");
		GL_ASSERT(location != -1);
		for(auto& el : m_lines)
			el.SetShaderUniformsProgram([location]()
			{glUniform4f(location, .0f, .0f, .0f, .0f);});
	}

	for(auto& el : m_lines)
		el.Init();
}

void Jelly::Draw()
{
	for (auto& el : m_lines)
		el.Draw();
}



void Jelly::Tugging(float xCoefficient, float yCoefficient)
{
	
}

