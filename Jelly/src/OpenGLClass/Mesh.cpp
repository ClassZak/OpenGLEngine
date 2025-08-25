#include "Mesh.hpp"

void Mesh::Init(const std::vector<Vertex3DNormText>& vertices, const std::vector<unsigned int>& indexes)
{
	VertexBufferObject* newVertexBufferObject =
	new VertexBufferObject(vertices.data(), vertices.size() * sizeof(Vertex3DNormText), vertices.size());
	this->m_vertexBufferObject.reset(newVertexBufferObject);

	// Attribute crafting
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);

	IndexBufferObject* newIndexBufferObject = new IndexBufferObject(indexes);
	this->m_indexBufferObject.reset(newIndexBufferObject);
}

Mesh::Mesh
(
	const std::vector<Vertex3DNormText>& vertices, 
	const std::vector<unsigned int>& indexes, 
	const std::shared_ptr<Texture> texture,
	const std::shared_ptr<Shader> shader
) : 
IHasVertexVector<Vertex3DNormText>(vertices), IHasTexture(texture), IHasShader(shader)
{
	Init(vertices, indexes);
}
