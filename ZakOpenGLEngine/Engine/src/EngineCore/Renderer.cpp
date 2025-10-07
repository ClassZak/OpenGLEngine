#include "../../include/ZakEngine/EngineCore/Renderer.hpp"

using namespace Zak;
Renderer& Renderer::GetInstance()
{
	//Singleton из C++11
	static Renderer instance;
	return instance;
}




Renderer& Renderer::Draw(const VertexBufferObject& vertexBufferObject)
{
	vertexBufferObject.Bind();
	GLLogCall(glDrawElements(GL_TRIANGLES, vertexBufferObject.GetSize(), GL_UNSIGNED_INT, nullptr));

	return *this;
}
Renderer& Renderer::Draw
(const VertexBufferObject& vertexBufferObject, const IndexBufferObject& indexBufferObject)
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
	const Uniform& uniform
)
{
	vertexArrayObject.Bind();
	vertexBufferObject.Bind();
	indexBufferObject.Bind();
	shader.Bind();
	shader.SetUniform(uniform);
	GLLogCall(glDrawElements(GL_TRIANGLES, indexBufferObject.GetCount(), GL_UNSIGNED_INT, nullptr));

	return *this;
}
Renderer& Renderer::Draw
(
	const VertexArrayObject& vertexArrayObject,
	const VertexBufferObject& vertexBufferObject,
	const IndexBufferObject& indexBufferObject,
	std::shared_ptr<Texture> texture,
	Shader& shader,
	const Uniform& uniform
)
{
	shader.Bind();
	shader.SetUniform(uniform);

	Texture* texturePtr = texture.get();
	GLLogCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLLogCall(glEnable(GL_BLEND));
	texturePtr->Bind(0);
	
	vertexArrayObject.Bind();
	vertexBufferObject.Bind();
	indexBufferObject.Bind();
	GLLogCall(glDrawElements(GL_TRIANGLES, indexBufferObject.GetCount(), GL_UNSIGNED_INT, nullptr));

	texturePtr->UnBind();

	return *this;
}
Renderer& Renderer::Draw(IDrawableOpenGL* object)
{
	auto* iHasVertexArrayObject = dynamic_cast<IHasVertexArrayObject*>(object);
	auto* iHasVertexBufferObject = dynamic_cast<IHasVertexBufferObject*>(object);
	auto* iHasIndexBufferObject = dynamic_cast<IHasIndexBufferObject*>(object);
	
	
	if (not (iHasIndexBufferObject || iHasVertexBufferObject))
		throw std::invalid_argument("Wrong type");
		
	if(iHasVertexArrayObject)
		iHasVertexArrayObject->GetVertexArrayObject().Bind();
	if(iHasVertexBufferObject)
		iHasVertexBufferObject->GetVertexBufferObject()->Bind();
	if(iHasIndexBufferObject)
		iHasIndexBufferObject->GetIndexBufferObject()->Bind();

	// Фигурные скобки обязательно
	if (iHasIndexBufferObject)
	{
		GLLogCall
		(
			glDrawElements
			(
				object->GetDrawMode(),
				iHasIndexBufferObject->GetIndexBufferObject()->GetCount(),
				GL_UNSIGNED_INT,
				nullptr
			)
		);
	}
	else if(iHasVertexBufferObject)
	{
		GLLogCall
		(glDrawArrays(object->GetDrawMode(), 0, iHasVertexBufferObject->GetVertexBufferObject()->GetSize()));
	}

	return *this;
}



Renderer& Renderer::Draw(IDrawableOpenGL* object, const std::vector<Uniform>& uniforms)
{
	GLsizeiptr size = 0;
	auto* iHasIndexBufferObject = dynamic_cast<IHasIndexBufferObject*>(object);
	auto* iHasVertexBufferObject = dynamic_cast<IHasVertexBufferObject*>(object);
	auto* iHasVertexArrayObject = dynamic_cast<IHasVertexArrayObject*>(object);
	auto* iHasShader = dynamic_cast<IHasShader*>(object);
	auto* iHasTexture = dynamic_cast<IHasTexture*>(object);

	if (!iHasVertexBufferObject)
		throw std::invalid_argument("Unable to render object without vertices");
	if (iHasTexture && !iHasShader)
		throw std::invalid_argument("Unable to render texture without shader");

	if (iHasIndexBufferObject)
		size = iHasIndexBufferObject->GetIndexBufferObject()->GetCount();
	else if (iHasVertexBufferObject)
		size = iHasVertexBufferObject->GetVertexBufferObject()->GetCount();
	else
		throw std::invalid_argument("Wrong type");

	if (iHasVertexArrayObject)
		iHasVertexArrayObject->GetVertexArrayObject().Bind();
	if (iHasVertexBufferObject)
		iHasVertexBufferObject->GetVertexBufferObject()->Bind();
	if (iHasIndexBufferObject)
		iHasIndexBufferObject->GetIndexBufferObject()->Bind();


	if (iHasShader)
	{
		if (Shader* shader = iHasShader->GetShaderSharedPointer())
		{
			shader->Bind();
			for (auto it = uniforms.begin(); it != uniforms.end(); ++it)
				shader->SetUniform(*it);


			if (iHasTexture)
			{
				Texture* texture = iHasTexture->GetTextureSharedPointer();
				if (texture)
				{
					texture->Bind(0);
					shader->SetUniform(Uniform("u_Texture", 0));
					GLLogCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
					GLLogCall(glEnable(GL_BLEND));
				}
			}
		}
	}

	if (object->GetDrawMode() != GL_TRIANGLES)
	{
		if (object->GetDrawMode() == GL_LINE_STRIP)
		{
			if (Line<float>* line = dynamic_cast<Line<float>*>(object))
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
		iHasShader->GetShaderSharedPointer()->UnBind();
	if (iHasTexture)
		if(iHasTexture->GetTextureSharedPointer())
			iHasTexture->GetTextureSharedPointer()->UnBind();

	return *this;
}