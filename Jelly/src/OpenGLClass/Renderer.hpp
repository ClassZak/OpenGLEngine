#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <list>
#include <unordered_map>
#include <sstream>
#include <cctype>
#include <utility>
#include <array>
#include <vector>
#include <set>

#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBufferObject.hpp"
#include "../OpenGLClass/Shader.hpp"
#include "../Shape/Interfaces/IHasIndexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexArrayObject.hpp"
#include "../Shape/Interfaces/IHasShader.hpp"
#include "../Shape/Interfaces/IDrawable.hpp"

#include "../Shape/Line.hpp"

class Renderer
{
	Renderer() = default;
	
	
	std::set<Shader> m_shaders;
public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	
	static Renderer& GetInstance();
	
	void AddShader(const Shader& shader);
	void AddShader(const std::string& filepath);
	std::set<Shader>& GetShaders()
	{
		return m_shaders;
	}
	
	Renderer& Draw(const VertexBufferObject& vertexBufferObject);
	Renderer& Draw(const VertexBufferObject& vertexBufferObject, const IndexBufferObject& indexBufferObject);
	Renderer& Draw
	(
		const VertexArrayObject& vertexArrayObject,
		const VertexBufferObject& vertexBufferObject,
		const IndexBufferObject& indexBufferObject
	);
	Renderer& Draw
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
	Renderer& Draw(IDrawableOpenGL* object);



	
	Renderer& Draw(IDrawableOpenGL* object, const Uniform& uniform)
	{
		std::vector<Uniform> uniformsVector;
		uniformsVector.emplace_back(uniform);

		return Draw(object, uniformsVector);
	}
	Renderer& Draw(IDrawableOpenGL* object, const std::initializer_list<Uniform>& uniforms)
	{
		std::vector<Uniform> uniformsVector(uniforms);

		return Draw(object, uniformsVector);
	}
	Renderer& Draw(IDrawableOpenGL* object, const std::vector<Uniform>& uniforms)
	{
		GLsizeiptr size = 0;
		auto* iHasIndexBufferObject = dynamic_cast<IHasIndexBufferObject*>(object);
		auto* iHasVertexBufferObject = dynamic_cast<IHasVertexBufferObject*>(object);
		auto* iHasVertexArrayObject = dynamic_cast<IHasVertexArrayObject*>(object);
		auto* iHasShader = dynamic_cast<IHasShader*>(object);

		if (!iHasVertexBufferObject)
			throw std::invalid_argument("Unable to render object without vertecies");

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
			Shader* shader = iHasShader->GetShader();
			shader->Bind();
			for (auto it = uniforms.begin(); it != uniforms.end(); ++it)
				shader->SetUniform(*it);
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
};


