#include "Jelly.hpp"

inline void Jelly::AnimateLines(long long millisecondsSinceEpoch, double animationSpeed)
{
	const auto ANIMATION_COEFFICIENT = millisecondsSinceEpoch * animationSpeed;

	auto animatedIt = m_lines.begin();
	auto createdIt = m_createdLines.begin();

	// Ey ý [0;1]
	const double SHORT_COS_COEFFICIENT = (cos(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_BORDER_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT / 10) + 1) / 2.;
	// Ey ý [-1;1]
	const double COS_COEFFICIENT = cos(ANIMATION_COEFFICIENT);
	const double SIN_COEFFICIENT = sin(ANIMATION_COEFFICIENT);

	while (createdIt != m_createdLines.end() && animatedIt != m_lines.end())
	{
		auto& vertexes = animatedIt->GetVertices();
		auto& created_vertexes = createdIt->GetVertices();


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

	// Ey ý [0;1]
	const double SHORT_COS_COEFFICIENT = (cos(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_BORDER_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT / 10) + 1) / 2.;
	// Ey ý [-1;1]
	const double COS_COEFFICIENT = cos(ANIMATION_COEFFICIENT);
	const double SIN_COEFFICIENT = sin(ANIMATION_COEFFICIENT);

	while (createdIt != m_createdRoundedLines.end() && animatedIt != m_roundedLines.end())
	{
		auto& vertexes = animatedIt->GetVertices();
		auto& created_vertexes = createdIt->GetVertices();
		
		
		for (std::size_t i = 0; i != created_vertexes.size() && i != vertexes.size(); ++i)
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

	auto animatedIt = m_quadrangles.begin();
	auto createdIt = m_createdQuadrangles.begin();

	// Ey ý [0;1]
	const double SHORT_COS_COEFFICIENT = (cos(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_BORDER_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT / 10) + 1) / 2.;
	// Ey ý [-1;1]
	const double COS_COEFFICIENT = cos(ANIMATION_COEFFICIENT);
	const double SIN_COEFFICIENT = sin(ANIMATION_COEFFICIENT);

	while (createdIt != m_createdQuadrangles.end() && animatedIt != m_quadrangles.end())
	{
		auto& vertexes = animatedIt->GetVertices();
		auto& created_vertexes = createdIt->GetVertices();


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

	auto animatedIt = m_circleSectors.begin();
	auto createdIt = m_createdCircleSectors.begin();

	// Ey ý [0;1]
	const double SHORT_COS_COEFFICIENT = (cos(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT) + 1) / 2.;
	const double SHORT_BORDER_SIN_COEFFICIENT = (sin(ANIMATION_COEFFICIENT / 10) + 1) / 2.;
	// Ey ý [-1;1]
	const double COS_COEFFICIENT = cos(ANIMATION_COEFFICIENT);
	const double SIN_COEFFICIENT = sin(ANIMATION_COEFFICIENT);

	while (createdIt != m_createdCircleSectors.end() && animatedIt != m_circleSectors.end())
	{
		auto& vertexes = animatedIt->GetVertices();
		auto& created_vertexes = createdIt->GetVertices();


		for (std::size_t i = 0; i != created_vertexes.size() && i != vertexes.size(); ++i)
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
	for (int i = 0; i <= PARTS_COUNT; ++i)
	{
		float x= START_X + i * PART_SIZE;
		m_createdLines.push_back
		(Line<float>({ Vertex2D<float>(x,START_Y),Vertex2D<float>(x * MULTIPLE_COEFFICIENT,END_Y) }));
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
			ROUNDED_LINES_VERTEX_COUNT,
			upper_vertex_x_delta/2,
			Vertex2D<float>(upper_vertex_x + upper_vertex_x_delta/2.f ,END_Y), 0, M_PI
		);

		round_vertexes_list.push_back(vertexes);

		m_createdCircleSectors.push_back
		(
			CircleSector<float>
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
			m_createdRoundedLines.push_back(Line<float>({el[i],el[i+1]}));
	}

	for (auto& el : m_createdRoundedLines)
	{
		m_roundedLines.push_back(el);
	}

	for (auto& el : m_createdCircleSectors)
	{
		m_circleSectors.push_back(el);
	}




	for (auto& el : m_createdLines)
	{
		m_lines.push_back(el);
	}




	for (int i = 0; i < PARTS_COUNT; ++i)
	{
		float x = START_X + i * PART_SIZE;
		float next_x = START_X + (i + 1) * PART_SIZE;
		float upper_vertex_x = x * MULTIPLE_COEFFICIENT;
		float next_upper_vertex_x = next_x * MULTIPLE_COEFFICIENT;

		float upper_vertex_x_delta = abs(next_upper_vertex_x - upper_vertex_x);

		m_createdQuadrangles.push_back(
		{
			Vertex2D<float>(x,START_Y),
			Vertex2D<float>(upper_vertex_x , END_Y),
			Vertex2D<float>(next_upper_vertex_x ,END_Y),
			Vertex2D<float>(next_x,START_Y)
		});

		m_quadrangles.push_back(m_createdQuadrangles.back());
	}

	auto& rendererShader = Renderer::GetInstance().FindShader(3);
	m_bottomLine->SetShader(rendererShader);
	for(auto& el : m_lines)
		el.SetShader(rendererShader);
	for(auto& el : m_roundedLines)
		el.SetShader(rendererShader);
	for(auto& el : m_quadrangles)
		el.SetShader(rendererShader);
	for(auto& el : m_circleSectors)
		el.SetShader(rendererShader);
}

void Jelly::Draw()
{
	const Uniform RED_COLOR_UNIFORM = Uniform("u_Color", UniformVec4{ 1.f, 0.f, 0.f, 1.f });
	const Uniform BLACK_COLOR_UNIFORM = Uniform("u_Color", UniformVec4{ 0.f, 0.f, 0.f, 1.f });

	std::for_each
	(m_quadrangles.begin(), m_quadrangles.end(), [&RED_COLOR_UNIFORM](Quadrangle<float>& arg) -> void
		{Renderer::GetInstance().Draw(&arg, RED_COLOR_UNIFORM);});
	std::for_each
	(m_circleSectors.begin(), m_circleSectors.end(), [&RED_COLOR_UNIFORM](CircleSector<float>& arg) -> void
		{Renderer::GetInstance().Draw(&arg, RED_COLOR_UNIFORM);});


	Renderer::GetInstance().Draw(m_bottomLine, Uniform("u_Color", UniformVec4{ 0.f, 0.f, 0.f, 1.f }));


	std::for_each
	(m_lines.begin(), m_lines.end(), [&BLACK_COLOR_UNIFORM](Line<float>& arg) -> void
		{Renderer::GetInstance().Draw(&arg, BLACK_COLOR_UNIFORM); });
	std::for_each
	(m_roundedLines.begin(), m_roundedLines.end(), [&BLACK_COLOR_UNIFORM](Line<float>& arg) -> void
		{Renderer::GetInstance().Draw(&arg, BLACK_COLOR_UNIFORM); });
}


void Jelly::Animate(long long millisecondsSinceEpoch, double animationSpeed)
{
	AnimateLines(millisecondsSinceEpoch, animationSpeed);
	AnimateRoundedLines(millisecondsSinceEpoch, animationSpeed);
	AnimateParts(millisecondsSinceEpoch, animationSpeed);
	AnimateRoundedParts(millisecondsSinceEpoch, animationSpeed);

	for (auto& el : m_lines)
		el.ReBind();
	for (auto& el : m_roundedLines)
		el.ReBind();
	for (auto& el : m_quadrangles)
		el.ReBind();
	for (auto& el : m_circleSectors)
		el.ReBind();
}

