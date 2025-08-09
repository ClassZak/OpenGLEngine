#pragma once
#include "../OpenGLClass/Shader.hpp"
#include "../Shape/Interfaces/IHasIndexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexArrayObject.hpp"
#include "../Shape/Interfaces/IHasShader.hpp"
#include "../Shape/Interfaces/IDrawable.hpp"
#include "../Vertex/Vertex2D.hpp"

class SimpleRect : 
	public IHasVertexArrayObject, 
	public IHasVertexBufferObject, 
	public IHasIndexBufferObject, 
	public IHasShader
{
public:
	template<typename T>
	SimpleRect(const Vertex2D<T>& pos, T width, T height)
	{
		std::vector<Vertex2D<T>> vertexes =
		{
			pos,
			Vertex2D(pos.x, pos.y + height),
			pos + Vertex2D(width, height),
			Vertex2D(pos.x + width, pos.y),
		};

		VertexBufferObject* newVertexBufferObject = new VertexBufferObject(vertexes);
		this->m_vertexBufferObject.reset(newVertexBufferObject);

		// Attribute crafting
		VertexBufferLayout layout;
		layout.Push<T>(2);
		this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);

		IndexBufferObject* newIndexBufferObject = new IndexBufferObject({ 0, 1, 2, 0, 2, 3 });
		this->m_indexBufferObject.reset(newIndexBufferObject);

		this->SetShader("../res/shaders/shader.shader");
		this->m_shader.get()->Bind();
		this->m_shader.get()->SetUniform(Uniform("u_Color", UniformVec4{ 1.f, 0.f, 0.f, 1.f }));


		this->m_vertexArrayObject.UnBind();
		this->m_vertexBufferObject.get()->UnBind();
		this->m_indexBufferObject.get()->UnBind();
		this->m_shader.get()->UnBind();
	}
	
	
	operator IDrawableOpenGL* ()
	{
		return dynamic_cast<IHasVertexArrayObject*>(this);
	}
};
