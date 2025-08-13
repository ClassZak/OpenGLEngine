#include "Jelly.hpp"

inline void Jelly::AnimateLines(long long millisecondsSinceEpoch, double animationSpeed)
{
	const auto ANIMATION_COEFFICIENT = millisecondsSinceEpoch * animationSpeed;

	auto animatedIt = m_lines.begin();
	auto createdIt = m_createdLines.begin();

	// Ey э [0;1]
	const double SHORT_COS_COEFFICIENT = (cos(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_BORDER_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT / 10) + 1) / 2.;
	// Ey э [-1;1]
	const double COS_COEFFICIENT = cos(ANIMATION_COEFFICIENT);
	const double SIN_COEFFICIENT = sin(ANIMATION_COEFFICIENT);

	while (createdIt != m_createdLines.end() && animatedIt != m_lines.end())
	{
		auto& vertexes = animatedIt->GetVertexes();
		auto& created_vertexes = createdIt->GetVertexes();


		auto vertex_it = std::next(vertexes.begin());
		auto created_vertex_it = std::next(created_vertexes.begin());

		(*vertex_it).x =
		(*created_vertex_it).x * ((1-SHORT_COS_COEFFICIENT) * 0.2 * (1-SHORT_BORDER_SIN_COEFFICIENT) + 0.8);

		++createdIt;
		++animatedIt;
	}
}

inline void Jelly::AnimateRoundedLines(long long millisecondsSinceEpoch, double animationSpeed)
{
	const auto ANIMATION_COEFFICIENT = millisecondsSinceEpoch * animationSpeed;

	auto animatedIt = m_roundedLines.begin();
	auto createdIt = m_createdRoundedLines.begin();

	// Ey э [0;1]
	const double SHORT_COS_COEFFICIENT = (cos(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_BORDER_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT / 10) + 1) / 2.;
	// Ey э [-1;1]
	const double COS_COEFFICIENT = cos(ANIMATION_COEFFICIENT);
	const double SIN_COEFFICIENT = sin(ANIMATION_COEFFICIENT);

	while (createdIt != m_createdRoundedLines.end() && animatedIt != m_roundedLines.end())
	{
		auto& vertexes = animatedIt->GetVertexes();
		auto& created_vertexes = createdIt->GetVertexes();
		
		
		for (std::size_t i = 0; i != created_vertexes.size() and i != vertexes.size(); ++i)
		{
			vertexes[i].x =
			created_vertexes[i].x * 
			((1 - SHORT_COS_COEFFICIENT) * 0.2 * (1 - SHORT_BORDER_SIN_COEFFICIENT) + 0.8);
			

			float perpendicular = END_Y - created_vertexes[i].y;
			vertexes[i].y = END_Y - perpendicular * (SHORT_BORDER_SIN_COEFFICIENT * 0.4 + 0.6);
		}
		
		++createdIt;
		++animatedIt;
	}
}

inline void Jelly::AnimateParts(long long millisecondsSinceEpoch, double animationSpeed)
{
	const auto ANIMATION_COEFFICIENT = millisecondsSinceEpoch * animationSpeed;

	auto animatedIt = m_OldQuadrangles.begin();
	auto createdIt = m_createdOldQuadrangles.begin();

	// Ey э [0;1]
	const double SHORT_COS_COEFFICIENT = (cos(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_BORDER_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT / 10) + 1) / 2.;
	// Ey э [-1;1]
	const double COS_COEFFICIENT = cos(ANIMATION_COEFFICIENT);
	const double SIN_COEFFICIENT = sin(ANIMATION_COEFFICIENT);

	while (createdIt != m_createdOldQuadrangles.end() && animatedIt != m_OldQuadrangles.end())
	{
		auto& vertexes = animatedIt->GetVertexes();
		auto& created_vertexes = createdIt->GetVertexes();


		auto vertex_it = std::next(vertexes.begin(),1);
		auto created_vertex_it = std::next(created_vertexes.begin(),1);

		(*vertex_it).x =
		(*created_vertex_it).x * ((1 - SHORT_COS_COEFFICIENT) * 0.2 * (1 - SHORT_BORDER_SIN_COEFFICIENT) + 0.8);
		++vertex_it,++created_vertex_it;
		(*vertex_it).x =
		(*created_vertex_it).x * ((1 - SHORT_COS_COEFFICIENT) * 0.2 * (1 - SHORT_BORDER_SIN_COEFFICIENT) + 0.8);

		++createdIt;
		++animatedIt;
	}
}

inline void Jelly::AnimateRoundedParts(long long millisecondsSinceEpoch, double animationSpeed)
{
	const auto ANIMATION_COEFFICIENT = millisecondsSinceEpoch * animationSpeed;

	auto animatedIt = m_OldCircleSectors.begin();
	auto createdIt = m_createdOldCircleSectors.begin();

	// Ey э [0;1]
	const double SHORT_COS_COEFFICIENT = (cos(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_BORDER_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT / 10) + 1) / 2.;
	// Ey э [-1;1]
	const double COS_COEFFICIENT = cos(ANIMATION_COEFFICIENT);
	const double SIN_COEFFICIENT = sin(ANIMATION_COEFFICIENT);

	while (createdIt != m_createdOldCircleSectors.end() && animatedIt != m_OldCircleSectors.end())
	{
		auto& vertexes = animatedIt->GetVertexes();
		auto& created_vertexes = createdIt->GetVertexes();


		for (std::size_t i = 0; i != created_vertexes.size() and i != vertexes.size(); ++i)
		{
			vertexes[i].x =
				created_vertexes[i].x *
				((1 - SHORT_COS_COEFFICIENT) * 0.2 * (1 - SHORT_BORDER_SIN_COEFFICIENT) + 0.8);


			float perpendicular = END_Y - created_vertexes[i].y;
			vertexes[i].y = END_Y - perpendicular * (SHORT_BORDER_SIN_COEFFICIENT * 0.4 + 0.6);
		}

		++createdIt;
		++animatedIt;
	}
}






void Jelly::Init()
{
	m_bottomLine->Init();

	for (int i = 0; i <= PARTS_COUNT; ++i)
	{
		float x= START_X + i * PART_SIZE;
		m_createdLines.push_back
		(OldLine<float>({ Vertex2D(x,START_Y),Vertex2D(x * MULTIPLE_COEFFICIENT,END_Y) }));
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
		OldCircle<float>::GenerateRoundVertexes
		(
			ROUNDED_LINES_VERTEX_COUNT,
			upper_vertex_x_delta/2,
			Vertex2D<float>(upper_vertex_x + upper_vertex_x_delta/2.f ,END_Y), 0, M_PI
		);

		round_vertexes_list.push_back(vertexes);

		m_createdOldCircleSectors.push_back
		(
			OldCircleSector<float>
			(
				ROUNDED_LINES_VERTEX_COUNT,
				upper_vertex_x_delta / 2,
				Vertex2D<float>(upper_vertex_x + upper_vertex_x_delta / 2.f,END_Y),
				0,
				M_PI
			)
		);
	}
	for (auto& el : round_vertexes_list)
	{
		for(std::size_t i=0; i+1<el.size();++i)
			m_createdRoundedLines.push_back(OldLine({el[i],el[i+1]}));
	}

	for (auto& el : m_createdRoundedLines)
	{
		m_roundedLines.push_back(el);
		m_roundedLines.back().Init();
	}

	for (auto& el : m_createdOldCircleSectors)
	{
		m_OldCircleSectors.push_back(el);
		m_OldCircleSectors.back().Init();
	}




	for (auto& el : m_createdLines)
	{
		m_lines.push_back(el);
		m_lines.back().Init();
	}




	for (int i = 0; i < PARTS_COUNT; ++i)
	{
		float x = START_X + i * PART_SIZE;
		float next_x = START_X + (i + 1) * PART_SIZE;
		float upper_vertex_x = x * MULTIPLE_COEFFICIENT;
		float next_upper_vertex_x = next_x * MULTIPLE_COEFFICIENT;

		float upper_vertex_x_delta = abs(next_upper_vertex_x - upper_vertex_x);

		m_createdOldQuadrangles.push_back(
		{
			Vertex2D(x,START_Y),
			Vertex2D(upper_vertex_x , END_Y),
			Vertex2D(next_upper_vertex_x ,END_Y),
			Vertex2D(next_x,START_Y)
		});

		m_OldQuadrangles.push_back(m_createdOldQuadrangles.back());
		m_OldQuadrangles.back().Init();
	}


	if (m_shaderProgram)
	{
		int location = glGetUniformLocation(m_shaderProgram, "u_Color");
		GL_ASSERT(location != -1);
		
		// Для очистки от предыдущего использования
		m_bottomLine->SetShaderUniformsProgram([location]()
			{glUniform4f(location, .0f, .0f, .0f, .0f); });

		for(auto& el : m_OldQuadrangles)
			el.SetShaderUniformsProgram([location]()
				{glUniform4f(location, 1., .0f, .0f, .0f); });

		for(auto& el : m_OldCircleSectors)
			el.SetShaderUniformsProgram([location]()
				{glUniform4f(location, 1., .0f, .0f, .0f); });
	}
}

void Jelly::Draw()
{
	for(auto& el : m_OldQuadrangles)
		el.Draw();
	for(auto& el : m_OldCircleSectors)
		el.Draw();


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
	AnimateParts(millisecondsSinceEpoch, animationSpeed);
	AnimateRoundedParts(millisecondsSinceEpoch, animationSpeed);
}

