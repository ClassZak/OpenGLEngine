#include "Jelly.hpp"

Jelly::Jelly()
{
}



void Jelly::Init()
{
	m_createdLines.push_back
	(Line<float>({Vertex2D(START_X,START_Y),Vertex2D(END_X,START_Y) }));


	for (int i = 0; i <= PARTS_COUNT; ++i)
	{
		float x= START_X + i * LENGTH / PARTS_COUNT;
		m_createdLines.push_back
		(Line<float>({ Vertex2D(x,START_Y),Vertex2D(x * MULTIPLE_COEFFICIENT,END_Y) }));
	}





	if (m_shaderProgram)
	{
		int location = glGetUniformLocation(m_shaderProgram, "u_Color");
		GL_ASSERT(location != -1);
		for(auto& el : m_createdLines)
			el.SetShaderUniformsProgram([location]()
			{glUniform4f(location, .0f, .0f, .0f, .0f);});
	}


	for (auto& el : m_createdLines)
	{
		m_lines.push_back(el);
		m_lines.back().Init();
	}

	for (auto& el : m_createdLines)
		el.Init();
}

void Jelly::Draw()
{
	for (auto& el : m_lines)
		el.Draw();
}



void Jelly::Tugging(float xCoefficient, float yCoefficient)
{
	auto lineIt = m_lines.begin();
	auto createdIt = m_createdLines.begin();

	while (createdIt != m_createdLines.end() && lineIt != m_lines.end())
	{
		auto& lineVertexes = lineIt->GetVertexes();
		auto& createdLineVertexes = createdIt->GetVertexes();
		

		auto lineVertexIt = lineVertexes.begin();
		auto createdLineVertexIt = createdLineVertexes.begin();

		while
		(
			lineVertexIt != lineVertexes.end() && 
			createdLineVertexIt != createdLineVertexes.end()
		)
		{
			(*lineVertexIt).x = (*createdLineVertexIt).x + (rand()%10) / 1000.;
			(*lineVertexIt).y = (*createdLineVertexIt).y + (rand()%10) / 100.;

			++lineVertexIt;
			++createdLineVertexIt;
		}
		++createdIt;
		++lineIt;
	}
}

