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
Renderer& Renderer::Draw
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
Renderer& Renderer::Draw(IDrawableOpenGL* object, const std::string& uniformName, const Uniform_4f& uniform_4f)
{
	GLsizeiptr size = 0;
	auto* iHasIndexBufferObject		= dynamic_cast<IHasIndexBufferObject*>(object);
	auto* iHasVertexBufferObject	= dynamic_cast<IHasVertexBufferObject*>(object);
	auto* iHasVertexArrayObject		= dynamic_cast<IHasVertexArrayObject*>(object);
	auto* iHasShader				= dynamic_cast<IHasShader*>(object);

	if(!iHasVertexBufferObject)
		throw std::invalid_argument("Unable to render object without vertecies");

	if (iHasIndexBufferObject)
		size = iHasIndexBufferObject->GetIndexBufferObject()->GetCount();
	else if(iHasVertexBufferObject)
		size = iHasVertexBufferObject->GetVertexBufferObject()->GetCount();
	else 
		throw std::invalid_argument("Wrong type");
	
	if(iHasVertexArrayObject)
		iHasVertexArrayObject->GetVertexArrayObject().Bind();
	if(iHasVertexBufferObject)
		iHasVertexBufferObject->GetVertexBufferObject()->Bind();
	if(iHasIndexBufferObject)
		iHasIndexBufferObject->GetIndexBufferObject()->Bind();
	
	if(iHasShader)
	{
		Shader* shader = iHasShader->GetShader();
		shader->Bind();
		shader->SetUniform_4f(uniformName, uniform_4f.v0, uniform_4f.v1, uniform_4f.v2, uniform_4f.v3);
	}
	
	if (object->GetDrawMode() != GL_TRIANGLES)
	{
		if (object->GetDrawMode() == GL_LINE_STRIP)
		{
			if (Line* line = dynamic_cast<Line*>(object))
			{
				// m_lineWidth != 1
				if (abs((double)line->GetWidth() - 1) > 1e-10)
					glLineWidth(line->GetWidth());
				if (line->IsSmooth())
				{
					glEnable(GL_LINE_SMOOTH);
					glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
				}
			}
		}
	}


	// Фигурные скобки обязательно
	if (iHasIndexBufferObject)
	{
		GLLogCall(glDrawElements(object->GetDrawMode(), size, GL_UNSIGNED_INT, nullptr));
	}
	else
	{
		GLLogCall(glDrawArrays(object->GetDrawMode(), 0, size));
	}
	
	if (object->GetDrawMode() == GL_LINE_STRIP)
		glDisable(GL_LINE_SMOOTH);


	if (iHasVertexArrayObject)
		iHasVertexArrayObject->GetVertexArrayObject().UnBind();
	if (iHasVertexBufferObject)
		iHasVertexBufferObject->GetVertexBufferObject()->UnBind();
	if (iHasIndexBufferObject)
		iHasIndexBufferObject->GetIndexBufferObject()->UnBind();
	if (iHasShader)
		iHasShader->GetShader()->UnBind();

	return *this;
}



