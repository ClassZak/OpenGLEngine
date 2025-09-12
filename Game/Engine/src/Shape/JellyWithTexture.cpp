#define _USE_MATH_DEFINES
#include "../../include/ZakEngine/Shape/JellyWithTexture.hpp"


#include "../../include/ZakEngine/Shape/CircleSector.hpp"

#include "../../include/ZakEngine/OpenGLClass/VertexArrayObject.hpp"
#include "../../include/ZakEngine/OpenGLClass/VertexBufferObject.hpp"
#include "../../include/ZakEngine/OpenGLClass/VertexBufferLayout.hpp"
#include "../../include/ZakEngine/OpenGLClass/IndexBufferObject.hpp"
#include "../../include/ZakEngine/OpenGLClass/Shader.hpp"
#include "../../include/ZakEngine/OpenGLClass/Texture.hpp"

#include "../../include/ZakEngine/Vertex/Vertex2DText.hpp"
#include "../../include/ZakEngine/Vertex/Vertex2D.hpp"

#include <unordered_set>
#include <algorithm>
#include <ranges>
#include <chrono>
#include <stdlib.h>

using namespace Zak;

void JellyWithTexture::Init()
{
#ifdef _DEBUG
	std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
#endif
	VertexArrayObject vertex_array_object;

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
		vertexes.insert
		(vertexes.begin(), Vertex2D<float>(upper_vertex_x + upper_vertex_x_delta / 2.f, END_Y));

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
		if(std::find(this->m_vertices.begin(), this->m_vertices.end(), el) == this->m_vertices.end())
			this->m_vertices.push_back(el);
	for(auto& vector : circle_vertices)
		for(auto& el : vector)
				this->m_vertices.push_back(el);




	std::vector<unsigned int> indexes;
	const unsigned int FIRST_INDEX_OF_RECT_VERTEX = 0;
	unsigned int current_index = FIRST_INDEX_OF_RECT_VERTEX;
	for (int i = 0; i < PARTS_COUNT; ++i)
	{
		indexes.emplace_back(current_index + 0);
		indexes.emplace_back(current_index + 1);
		indexes.emplace_back(current_index + 3);

		indexes.emplace_back(current_index + 0);
		indexes.emplace_back(current_index + 2);
		indexes.emplace_back(current_index + 3);

		current_index +=2;
	}
	current_index+=2;
	for (auto& vector : circle_vertices)
	{
		std::vector<unsigned int> circle_vertices_indexes = 
		CircleSector<float>::GenerateCircleSectorVertexIndexes(ROUNDED_LINES_VERTEX_COUNT);
		for (auto& el : circle_vertices_indexes)
		{
			el += current_index;
		}
		std::copy
		(circle_vertices_indexes.begin(), circle_vertices_indexes.end(), std::back_inserter(indexes));
		current_index+=vector.size();
	}


	std::copy(this->m_vertices.begin(), this->m_vertices.end(), std::back_inserter(m_generated_vertices));
#ifdef _DEBUG
	std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
	std::cout<<"Generated for\t"
			<<std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() 
			<<"ms" << std::endl;
#endif
	VertexBufferObject* newVertexBufferObject =
	new VertexBufferObject
	(this->m_vertices.data(), this->m_vertices.size() * sizeof(Vertex2DText), this->m_vertices.size());
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
	double animation_coefficient= millisecondsSinceEpoch * animationSpeed;
	for (size_t i = 0; i < this->m_vertices.size(); ++i)
	{
		this->m_vertices[i].m_pos.x = 
		m_generated_vertices[i].m_pos.x + cos(animation_coefficient) / (rand() % 200);
		this->m_vertices[i].m_pos.y =
		m_generated_vertices[i].m_pos.y + sin(animation_coefficient) / (rand() % 200);
	}
	this->ReBind();
}




inline Vertex2DText JellyWithTexture::CreateVertexText(float x, float y)
{
	return Vertex2DText({x, y}, {Texture::OpenGLCoordToTextCoord(x),Texture::OpenGLCoordToTextCoord(y)});
}