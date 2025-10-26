#pragma once

#include <vector>
#include <memory>

#include <ZakEngine/Shape/Interfaces/IHasVertexVector.hpp>

#include <ZakEngine/Shape/Interfaces/IHasVertexArrayObject.hpp>
#include <ZakEngine/Shape/Interfaces/IHasVertexBufferObject.hpp>
#include <ZakEngine/Shape/Interfaces/IHasIndexBufferObject.hpp>
#include <ZakEngine/Shape/Interfaces/IHasShader.hpp>
#include <ZakEngine/Shape/Interfaces/IHasTexture.hpp>


namespace Zak {
class Vertex2DText;

class VertexArrayObject;
class VertexBufferObject;
class VertexBufferLayout;
class IndexBufferObject;
class Shader;
class Texture;
}
class JellyWithTexture :
	virtual public Zak::IHasVertexVector<Zak::Vertex2DText>,
	virtual public Zak::IHasTexture,
	virtual public Zak::IHasShader,
	virtual public Zak::IHasVertexArrayObject,
	virtual public Zak::IHasVertexBufferObject,
	virtual public Zak::IHasIndexBufferObject
{
	std::weak_ptr<Zak::Texture> m_texture;

	void Init();


	const float START_X = -0.75f;
	const float END_X = 0.75f;

	const float START_Y = -0.75f;
	const float END_Y = 0.5f;

	const float LENGTH = abs(START_X - END_X);

	const unsigned int PARTS_COUNT = 5u;
	const float MULTIPLE_COEFFICIENT = 0.9f;

	const float PART_SIZE = LENGTH / PARTS_COUNT;


	const float ROUNDED_LINES_VERTEX_COUNT = 30;



	std::vector<Zak::Vertex2DText> m_generated_vertices;
public:
	JellyWithTexture(std::shared_ptr<Zak::Texture> texture, std::shared_ptr<Zak::Shader> shader);
	/// <summary>
	/// Метод для дёрганья желе по времени
	/// </summary>
	/// <param name="millisecondsSinceEpoch">время</param>
	void Animate(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);

	static inline Zak::Vertex2DText CreateVertexText(float x, float y);
};