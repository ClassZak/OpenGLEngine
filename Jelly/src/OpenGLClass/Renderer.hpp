#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <list>
#include <unordered_map>
#include <memory>
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
	
	
	std::set<std::shared_ptr<Shader>> m_shaders;
public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	
	static Renderer& GetInstance();
	
	void AddShader(std::shared_ptr<Shader> shader);
	void AddShader(const std::string& filepath);
	std::set<std::shared_ptr<Shader>>& GetShaders()
	{
		return m_shaders;
	}
	inline const std::shared_ptr<Shader> FindShader(GLuint program)
	{
		auto it = std::ranges::find_if(m_shaders, [program](const std::shared_ptr<Shader>& shader)->bool
		{
			return shader.get()->GetProgram()==program;
		});
		if(it == m_shaders.end())
			return std::shared_ptr<Shader>(nullptr);
		else
			return *it;
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
	Renderer& Draw(IDrawableOpenGL* object, const std::vector<Uniform>& uniforms);
};

