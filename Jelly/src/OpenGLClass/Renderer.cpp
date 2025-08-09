#include "Renderer.hpp"

Renderer& Renderer::GetInstance()
{
	//Singleton из C++11
	static Renderer instance;
	return instance;
}


void Renderer::AddShader(const Shader& shader)
{
	m_shaders.insert(shader);
}
void Renderer::AddShader(const std::string& filepath)
{
	AddShader(Shader(filepath));
}



Renderer& Renderer::Draw(const VertexBufferObject& vertexBufferObject)
{
	vertexBufferObject.Bind();
	GLLogCall(glDrawElements(GL_TRIANGLES, vertexBufferObject.GetCount(), GL_UNSIGNED_INT, nullptr));

	return *this;
}
Renderer& Renderer::Draw(const VertexBufferObject& vertexBufferObject, const IndexBufferObject& indexBufferObject)
{
	vertexBufferObject.Bind();
	indexBufferObject.Bind();
	GLLogCall(glDrawElements(GL_TRIANGLES, indexBufferObject.GetCount(), GL_UNSIGNED_INT, nullptr));

	return *this;
}
Renderer& Renderer::Draw
(
	const VertexArrayObject& vertexArrayObject,
	const VertexBufferObject& vertexBufferObject,
	const IndexBufferObject& indexBufferObject
)
{
	vertexArrayObject.Bind();
	vertexBufferObject.Bind();
	indexBufferObject.Bind();
	GLLogCall(glDrawElements(GL_TRIANGLES, indexBufferObject.GetCount(), GL_UNSIGNED_INT, nullptr));

	return *this;
}
Renderer& Renderer::Draw(IDrawableOpenGL* object)
{
	if (auto* iHasVertexArrayObject = dynamic_cast<IHasVertexArrayObject*>(object))
		iHasVertexArrayObject->GetVertexArrayObject().Bind();
	if (auto* iHasVertexBufferObject = dynamic_cast<IHasVertexBufferObject*>(object))
		iHasVertexBufferObject->GetVertexBufferObject()->Bind();
	if (auto* iHasIndexBufferObject = dynamic_cast<IHasIndexBufferObject*>(object))
		iHasIndexBufferObject->GetIndexBufferObject()->Bind();

	return *this;
}


