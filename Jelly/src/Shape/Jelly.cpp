#include "Jelly.hpp"

inline void Jelly::AnimateLines(long long millisecondsSinceEpoch, double animationSpeed)
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

		(*lineVertexIt).x =
		(*createdLineVertexIt).x * ((1-SHORT_COS_COEFFICIENT) * 0.2 * (1-SHORT_BORDER_SIN_COEFFICIENT) + 0.8);

		++createdIt;
		++lineIt;
	}
}

inline void Jelly::AnimateRoundedLines(long long millisecondsSinceEpoch, double animationSpeed)
{
	const auto ANIMATION_COEFFICIENT = millisecondsSinceEpoch * animationSpeed;

	auto lineIt = m_roundedLines.begin();
	auto createdIt = m_createdRoundedLines.begin();

	// Ey ý [0;1]
	const double SHORT_COS_COEFFICIENT = (cos(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_BORDER_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT / 10) + 1) / 2.;
	// Ey ý [-1;1]
	const double COS_COEFFICIENT = cos(ANIMATION_COEFFICIENT);
	const double SIN_COEFFICIENT = sin(ANIMATION_COEFFICIENT);

	while (createdIt != m_createdRoundedLines.end() && lineIt != m_roundedLines.end())
	{
		auto& lineVertexes = lineIt->GetVertexes();
		auto& createdLineVertexes = createdIt->GetVertexes();

		for (std::size_t i = 0; i != createdLineVertexes.size() and i != lineVertexes.size(); ++i)
		{
			/*float x = START_X + i * PART_SIZE;
			float next_x = START_X + (i + 1) * PART_SIZE;
			float upper_vertex_x = x * MULTIPLE_COEFFICIENT;
			float next_upper_vertex_x = next_x * MULTIPLE_COEFFICIENT;

			float upper_vertex_x_delta = abs(next_upper_vertex_x - upper_vertex_x);*/



			lineVertexes[i].x =
			createdLineVertexes[i].x * 
			((1 - SHORT_COS_COEFFICIENT) * 0.2 * (1 - SHORT_BORDER_SIN_COEFFICIENT) + 0.8);
		}

		++createdIt;
		++lineIt;
	}
}






void Jelly::Init()
{
	m_bottomLine->Init();



	for (int i = 0; i <= PARTS_COUNT; ++i)
	{
		float x= START_X + i * PART_SIZE;
		m_createdLines.push_back
		(Line<float>({ Vertex2D(x,START_Y),Vertex2D(x * MULTIPLE_COEFFICIENT,END_Y) }));
	}




	std::list<std::vector<Vertex2D<float>>> round_vertexes_list;
	for (int i = 0; i < PARTS_COUNT; ++i)
	{
		float x= START_X + i * PART_SIZE;
		float next_x= START_X + (i+1) * PART_SIZE;
		float upper_vertex_x = x * MULTIPLE_COEFFICIENT;
		float next_upper_vertex_x = next_x * MULTIPLE_COEFFICIENT;

		float upper_vertex_x_delta = abs(next_upper_vertex_x - upper_vertex_x);

		auto vertexes = 
		Circle<float>::GenerateRoundVertexes
		(
			20u,
			upper_vertex_x_delta/2,
			Vertex2D<float>(upper_vertex_x + upper_vertex_x_delta/2.f ,END_Y), 0, M_PI
		);
		for (auto& el : vertexes)
			std::cout<<el<<"\n";

		round_vertexes_list.push_back(vertexes);
	}
	for (auto& el : round_vertexes_list)
	{
		for(std::size_t i=0; i+1<el.size();++i)
			m_createdRoundedLines.push_back(Line({el[i],el[i+1]}));
	}


	for (auto& el : m_createdRoundedLines)
	{
		el.Init();
		m_roundedLines.push_back(el);
		m_roundedLines.back().Init();
	}




	if (m_shaderProgram)
	{
		int location = glGetUniformLocation(m_shaderProgram, "u_Color");
		GL_ASSERT(location != -1);
		
		
		m_bottomLine->SetShaderUniformsProgram([location]()
			{glUniform4f(location, .0f, .0f, .0f, .0f); });
		
		for(auto& el : m_createdLines)
			el.SetShaderUniformsProgram([location]()
			{glUniform4f(location, .0f, .0f, .0f, .0f);});
	}
	
	
	
	
	
	
	
	for (auto& el : m_createdLines)
	{
		el.Init();
		m_lines.push_back(el);
		m_lines.back().Init();
	}
}

void Jelly::Draw()
{
	m_bottomLine->Draw();

	for (auto& el : m_lines)
		el.Draw();
	for (auto& el : m_roundedLines)
		el.Draw();
}


void Jelly::Animate(long long millisecondsSinceEpoch, double animationSpeed)
{
	AnimateLines(millisecondsSinceEpoch, animationSpeed);
	AnimateRoundedLines(millisecondsSinceEpoch, animationSpeed);
}

