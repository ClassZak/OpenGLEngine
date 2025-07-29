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

class Renderer
{
	Renderer() = default;
	
	
	std::list<Shader> m_shaders;
public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	
	static Renderer& GetInstance();
	
	void AddShader(const Shader& shader);
	void AddShader(const std::string& filepath);
	std::list<Shader>& GetShaders()
	{
		return m_shaders;
	}
	
	void Draw(const VertexBufferObject& vertexBufferObject);
	void Draw(const VertexBufferObject& vertexBufferObject, const IndexBufferObject& indexBufferObject);
	void Draw
	(
		const VertexArrayObject& vertexArrayObject,
		const VertexBufferObject& vertexBufferObject,
		const IndexBufferObject& indexBufferObject
	);
	void Draw
	(
		const VertexArrayObject& vertexArrayObject,
		const VertexBufferObject& vertexBufferObject,
		const IndexBufferObject& indexBufferObject,
		Shader& shader,
		const std::string& uniformName,
		const Uniform_4f& uniform_4f
	);
};


