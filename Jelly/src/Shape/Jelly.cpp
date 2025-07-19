#include "Jelly.hpp"

Jelly::Jelly()
{
}



void Jelly::Init()
{
	m_bottomLine.Init();



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
	m_bottomLine.Draw();

	for (auto& el : m_lines)
		el.Draw();
}


void Jelly::Animate(long long millisecondsSinceEpoch, double animationSpeed)
{
	const auto ANIMATION_COEFFICIENT = millisecondsSinceEpoch * animationSpeed;

	auto lineIt = m_lines.begin();
	auto createdIt = m_createdLines.begin();

	// Ey ý [0;1]
	const double SHORT_COS_COEFFICIENT = (cos(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_BORDER_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT / 10) + 1) / 2.;
	// Ey ý [-1;1]
	const double COS_COEFFICIENT = cos(ANIMATION_COEFFICIENT);
	const double SIN_COEFFICIENT = sin(ANIMATION_COEFFICIENT);

	while (createdIt != m_createdLines.end() && lineIt != m_lines.end())
	{
		auto& lineVertexes = lineIt->GetVertexes();
		auto& createdLineVertexes = createdIt->GetVertexes();
		

		auto lineVertexIt = std::next(lineVertexes.begin());
		auto createdLineVertexIt = std::next(createdLineVertexes.begin());

		(*lineVertexIt).x = (*createdLineVertexIt).x * (SHORT_COS_COEFFICIENT * 0.3 * SHORT_BORDER_SIN_COEFFICIENT + 0.7);

		++createdIt;
		++lineIt;
	}
}

