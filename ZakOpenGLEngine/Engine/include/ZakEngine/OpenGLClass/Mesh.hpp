#pragma once

#include "../OpenGLClass/Texture.hpp"

#include "../Vertex/Vertex3DNormText.hpp"


#include "../Shape/Interfaces/IHasVertexArrayObject.hpp"
#include "../Shape/Interfaces/IHasVertexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexVector.hpp"
#include "../Shape/Interfaces/IHasIndexBufferObject.hpp"
#include "../Shape/Interfaces/IHasTexture.hpp"
#include "../Shape/Interfaces/IHasShader.hpp"

namespace Zak {
class Mesh :
	virtual public IHasVertexArrayObject,
	virtual public IHasVertexVector<Vertex3DNormText>,
	virtual public IHasIndexBufferObject,
	virtual public IHasTexture,
	virtual public IHasShader
{
protected:
	void Init(const std::vector<Vertex3DNormText>& vertices, const std::vector<unsigned int>& indexes);
public:
	Mesh
	(
		const std::vector<Vertex3DNormText>& vertices,
		const std::vector<unsigned int>& indexes,
		const std::shared_ptr<Texture> texture,
		const std::shared_ptr<Shader> shader
	);
	Mesh
	(
		const std::initializer_list<Vertex3DNormText>& vertices,
		const std::initializer_list<unsigned int>& indexes,
		const std::shared_ptr<Texture> texture,
		const std::shared_ptr<Shader> shader
	) :
	Mesh(std::vector<Vertex3DNormText>(vertices), std::vector<unsigned int>(indexes), texture, shader){}

	void Move(float x, float y, float z);
};
}