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
	SimpleRect()
	{
		std::vector<Vertex2D<float>> vertexes =
		{
			Vertex2D(-0.6f	,	0.6f),
			Vertex2D(0.6f	,	0.6f),
			Vertex2D(0.6f	,	-0.6f),
			Vertex2D(-0.6f	,	-0.6f),
		};
		
		this->SetVertexBufferObject(vertexes);
		VertexBufferLayout layout;
		layout.Push<float>(2);
		this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);
		this->SetIndexBufferObject({ 0, 1, 2, 0, 2, 3 });
		this->SetShader("../res/shaders/shader.shader");
		this->m_shader.get()->Bind();
		this->m_shader.get()->SetUniform_4f("u_Color", { 1.f, 0.f, 0.f, 1.f });

		this->m_vertexArrayObject.UnBind();
		this->m_vertexBufferObject.get()->UnBind();
		this->m_indexBufferObject.get()->UnBind();
		this->m_shader.get()->UnBind();
	}
};
