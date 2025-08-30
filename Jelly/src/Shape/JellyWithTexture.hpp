#pragma once

#include <vector>
#include <memory>

#include "Interfaces/IHasVertexVector.hpp"

#include "Interfaces/IHasVertexArrayObject.hpp"
#include "Interfaces/IHasVertexBufferObject.hpp"
#include "Interfaces/IHasIndexBufferObject.hpp"
#include "Interfaces/IHasShader.hpp"
#include "Interfaces/IHasTexture.hpp"



class Vertex2DText;

class VertexArrayObject;
class VertexBufferObject;
class VertexBufferLayout;
class IndexBufferObject;
class Shader;
class Texture;



class JellyWithTexture :
	virtual public IHasVertexVector<Vertex2DText>,
	virtual public IHasTexture,
	virtual public IHasShader,
	virtual public IHasVertexArrayObject,
	virtual public IHasVertexBufferObject,
	virtual public IHasIndexBufferObject
{
	std::weak_ptr<Texture> m_texture;

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



	std::vector<Vertex2DText> m_generated_vertices;
public:
	JellyWithTexture(std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader);
	/// <summary>
	/// Метод для дёрганья желе по времени
	/// </summary>
	/// <param name="millisecondsSinceEpoch">время</param>
	void Animate(long long millisecondsSinceEpoch, double animationSpeed = 1e-3);

	static inline Vertex2DText CreateVertexText(float x, float y);
};
