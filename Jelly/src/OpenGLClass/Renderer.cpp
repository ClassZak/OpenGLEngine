#include "Renderer.hpp"

Renderer& Renderer::GetInstance()
{
	//Singleton из C++11
	static Renderer instance;
	return instance;
}


void Renderer::AddShader(const Shader& shader)
{
	m_shaders.push_back(shader);
}
void Renderer::AddShader(const std::string& filepath)
{
	AddShader(Shader(filepath));
}



void Renderer::Draw(const VertexBufferObject& vertexBufferObject)
{
	vertexBufferObject.Bind();
	GLLogCall(glDrawElements(GL_TRIANGLES, vertexBufferObject.GetCount(), GL_UNSIGNED_INT, nullptr));
}
void Renderer::Draw(const VertexBufferObject& vertexBufferObject, const IndexBufferObject& indexBufferObject)
{
	vertexBufferObject.Bind();
	indexBufferObject.Bind();
	GLLogCall(glDrawElements(GL_TRIANGLES, indexBufferObject.GetCount(), GL_UNSIGNED_INT, nullptr));
}
void Renderer::Draw
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
}
void Renderer::Draw
(
	const VertexArrayObject& vertexArrayObject,
	const VertexBufferObject& vertexBufferObject,
	const IndexBufferObject& indexBufferObject,
	Shader& shader,
	const std::string& uniformName,
	const Uniform_4f& uniform_4f
)
{
	vertexArrayObject.Bind();
	vertexBufferObject.Bind();
	indexBufferObject.Bind();
	shader.Bind();
	shader.SetUniform_4f(uniformName, uniform_4f.v0, uniform_4f.v1, uniform_4f.v2, uniform_4f.v3);
	GLLogCall(glDrawElements(GL_TRIANGLES, indexBufferObject.GetCount(), GL_UNSIGNED_INT, nullptr));	
}
void Renderer::Draw(IDrawableOpenGL* object)
{
	if (auto* iHasVertexArrayObject = dynamic_cast<IHasVertexArrayObject*>(object))
		iHasVertexArrayObject->GetVertexArrayObject().Bind();
	if (auto* iHasVertexBufferObject = dynamic_cast<IHasVertexBufferObject*>(object))
		iHasVertexBufferObject->GetVertexBufferObject()->Bind();
	if (auto* iHasIndexBufferObject = dynamic_cast<IHasIndexBufferObject*>(object))
		iHasIndexBufferObject->GetIndexBufferObject()->Bind();
}
void Renderer::Draw(IDrawableOpenGL* object, const std::string& uniformName, const Uniform_4f& uniform_4f)
{
	GLsizeiptr size = 0;
	if (auto* iHasIndexBufferObject = dynamic_cast<IHasIndexBufferObject*>(object))
		size = iHasIndexBufferObject->GetIndexBufferObject()->GetCount();
	else if(auto* iHasVertexBufferObject = dynamic_cast<IHasVertexBufferObject*>(object))
		size = iHasVertexBufferObject->GetVertexBufferObject()->GetCount();
	else 
		throw std::invalid_argument("Wrong type");

	if (auto* iHasShader = dynamic_cast<IHasShader*>(object))
	{
		Shader* shder = iHasShader->GetShader();
		shder->Bind();
		shder->Bind();
		shder->SetUniform_4f(uniformName, uniform_4f.v0, uniform_4f.v1, uniform_4f.v2, uniform_4f.v3);
	}

	GLLogCall(glDrawElements(GL_TRIANGLES,size,GL_UNSIGNED_INT,nullptr));
}



