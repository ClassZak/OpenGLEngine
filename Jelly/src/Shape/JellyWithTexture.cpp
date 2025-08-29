#define _USE_MATH_DEFINES
#include "JellyWithTexture.hpp"


#include "CircleSector.hpp"

#include "../OpenGLClass/VertexArrayObject.hpp"
#include "../OpenGLClass/VertexBufferObject.hpp"
#include "../OpenGLClass/VertexBufferLayout.hpp"
#include "../OpenGLClass/IndexBufferObject.hpp"
#include "../OpenGLClass/Shader.hpp"
#include "../OpenGLClass/Texture.hpp"

#include "../Vertex/Vertex2DText.hpp"
#include "../Vertex/Vertex2D.hpp"

#include <unordered_set>
#include <algorithm>
#include <ranges>



void JellyWithTexture::Init()
{
	VertexArrayObject vertex_array_object;
	std::vector<Vertex2DText> all_vertices;

	std::vector<Vertex2DText> rects_vertices;
	for (int i = 0; i <= PARTS_COUNT; ++i)
	{
		float x = START_X + i * PART_SIZE;
		rects_vertices.push_back(CreateVertexText(x,START_Y));
		rects_vertices.push_back(CreateVertexText(x * MULTIPLE_COEFFICIENT,END_Y));
	}




	std::list<std::vector<Vertex2DText>> circle_vertices;
	std::list<std::vector<Vertex2D<float>>> generated_circle_vertices;
	for (int i = 0; i < PARTS_COUNT; ++i)
	{
		float x = START_X + i * PART_SIZE;
		float next_x = START_X + (i + 1) * PART_SIZE;
		float upper_vertex_x = x * MULTIPLE_COEFFICIENT;
		float next_upper_vertex_x = next_x * MULTIPLE_COEFFICIENT;

		float upper_vertex_x_delta = abs(next_upper_vertex_x - upper_vertex_x);

		auto vertexes =
			CircleSector<float>::GenerateCircleSectorVertexes
			(
				ROUNDED_LINES_VERTEX_COUNT,
				upper_vertex_x_delta / 2,
				Vertex2D<float>(upper_vertex_x + upper_vertex_x_delta / 2.f, END_Y), 0, M_PI
			);

		generated_circle_vertices.emplace_back(vertexes);
	}

	auto transform_function = []
		(const Vertex2D<float>& vertex) 
		{
			return CreateVertexText(vertex.x, vertex.y);
		};
	for (const auto& vector : generated_circle_vertices)
	{
		std::vector<Vertex2DText> new_vector;
		new_vector.reserve(vector.size());

		std::ranges::transform(vector, std::back_inserter(new_vector), transform_function);
		circle_vertices.emplace_back(new_vector);
	}


	for(auto& el : rects_vertices)
		if(std::find(all_vertices.begin(), all_vertices.end(), el) == all_vertices.end())
			all_vertices.push_back(el);
	for(auto& vector : circle_vertices)
		for(auto& el : vector)
			if (std::find(all_vertices.begin(), all_vertices.end(), el) == all_vertices.end())
				all_vertices.push_back(el);




	std::vector<unsigned int> indexes;
	const unsigned int FIRST_INDEX_OF_RECT_VERTEX = 0;
	unsigned int index_of_rect_vertex = FIRST_INDEX_OF_RECT_VERTEX;
	for (int i = 0; i < PARTS_COUNT; ++i)
	{
		indexes.emplace_back(index_of_rect_vertex + 0);
		indexes.emplace_back(index_of_rect_vertex + 1);
		indexes.emplace_back(index_of_rect_vertex + 3);

		indexes.emplace_back(index_of_rect_vertex + 0);
		indexes.emplace_back(index_of_rect_vertex + 2);
		indexes.emplace_back(index_of_rect_vertex + 3);

		index_of_rect_vertex +=2;
	}
	for (auto& el : rects_vertices)
		std::cout << el << '\n';
	std::cout << std::endl;
	for(auto& el : all_vertices)
		std::cout<<el<<'\n';
	std::cout<<std::endl;
	for (auto& el : indexes)
		std::cout << el << '\n';

	// TODO: Доделать вершины для закруглённых частей





	VertexBufferObject* newVertexBufferObject =
	new VertexBufferObject(all_vertices.data(), all_vertices.size() * sizeof(Vertex2DText), all_vertices.size());
	this->m_vertexBufferObject.reset(newVertexBufferObject);

	// Attribute crafting
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);

	IndexBufferObject* newIndexBufferObject = new IndexBufferObject(indexes);
	this->m_indexBufferObject.reset(newIndexBufferObject);

	this->m_vertexArrayObject.UnBind();
	this->m_vertexBufferObject.get()->UnBind();
	this->m_indexBufferObject.get()->UnBind();
}


JellyWithTexture::JellyWithTexture(std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader) :
IHasTexture(texture), IHasShader(shader)
{
	Init();
}

void JellyWithTexture::Animate(long long millisecondsSinceEpoch, double animationSpeed)
{
	
}

void JellyWithTexture::Draw()
{
	
}



inline Vertex2DText JellyWithTexture::CreateVertexText(float x, float y)
{
	return Vertex2DText({x, y}, {Texture::OpenGLCoordToTextCoord(x), Texture::OpenGLCoordToTextCoord(y)});
}

