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
#include <concepts>
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
#include "../Shape/ShapeWithUniform.hpp"

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
	void AddShader(const std::string& filepath, const std::string& name);
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
	inline const std::shared_ptr<Shader> FindShader(const std::string& name)
	{
		auto it = std::ranges::find_if(m_shaders, [&name](const std::shared_ptr<Shader>& shader)->bool
		{
			const char* shader_name = shader.get()->GetName();
			if(!shader_name)
				return false;
			return name == shader_name;
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





	/*---==== C++ 20 required ====----*/
	template <class Container>
		requires requires(const Container& c)
		{
			{ *std::begin(c) } -> std::convertible_to<IDrawableOpenGL*>;
		}

	inline Renderer& Draw(const Container& container)
	{
		for (auto& el : container)
			Draw(el);

		return *this;
	}


	template <class ShapeContainer, class UniformContainer>
		requires requires(const ShapeContainer& sc, const UniformContainer& uc)
		{
			{ *std::begin(sc) } -> std::convertible_to<IDrawableOpenGL*>;
			{ *std::begin(uc) } -> std::convertible_to<Uniform*>;
		}
	inline Renderer& Draw(const ShapeContainer& container, const UniformContainer& uniformContainer)
	{
		if(container.size() != uniformContainer.size())
			throw std::invalid_argument("Несоответсвие количества объектов числу униформ");

		auto containerIt = container.begin();
		auto uniformIt = uniformContainer.begin();
		while (containerIt != container.end() && uniformIt != uniformContainer.end())
		{
			Draw(*containerIt, **uniformIt);
			
			++containerIt;
			++uniformIt;
		}

		return *this;
	}


	template <class Container>
		requires requires(Container& c)
		{
			{ *std::begin(c) } -> std::convertible_to<IDrawableOpenGL*>;
		}
	inline Renderer& Draw(Container& container, const Uniform& uniform)
	{
		for (auto& el : container)
			Draw(el, uniform);

		return *this;
	}








	template <class ShapeContainer, typename T, class UniformContainer>
		requires requires(const ShapeContainer& sc, const UniformContainer& uc)
		{
			{ *std::begin(sc) } -> std::convertible_to<T>;
			{ *std::begin(uc) } -> std::convertible_to<Uniform>;
		}
	inline Renderer& DrawCollection(ShapeContainer& container, const UniformContainer& uniformContainer)
	{
		if (container.size() != uniformContainer.size())
			throw std::invalid_argument("Несоответсвие количества объектов числу униформ");

		auto containerIt = container.begin();
		auto uniformIt = uniformContainer.begin();
		while (containerIt != container.end() && uniformIt != uniformContainer.end())
		{
			Draw(&(*containerIt), *uniformIt);

			++containerIt;
			++uniformIt;
		}

		return *this;
	}
	template <class ShapeContainer, typename T>
		requires requires(const ShapeContainer& sc)
		{
			{ *std::begin(sc) } -> std::convertible_to<T>;
		}
	inline Renderer& DrawCollection(ShapeContainer& container)
	{
		for(auto& el : container)
			Draw(&el);

		return *this;
	}
	template <class ShapeContainer, typename T>
		requires requires(const ShapeContainer& sc)
		{
			{ *std::begin(sc) } -> std::convertible_to<T>;
		}
	inline Renderer& DrawCollection(ShapeContainer& container, const Uniform& uniform)
	{
		for(auto& el : container)
			Draw(&el, uniform);

		return *this;
	}
	/*---==== C++ 20 required ====----*/
};

