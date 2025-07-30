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
			Vertex2D(-0.5f	,	0.5f),
			Vertex2D(0.5f	,	0.5f),
			Vertex2D(0.5f	,	-0.5f),
			Vertex2D(-0.5f	,	-0.5f),
		};
		VertexArrayObject vertexArrayObject;
		VertexBufferObject vertexBuffer(vertexes);
		VertexBufferLayout layout;
		layout.Push<float>(2);
		vertexArrayObject.AddBuffer(vertexBuffer, layout);
		IndexBufferObject indexBufferObject({ 0, 1, 2, 0, 2, 3 });
		Shader shader = Shader("../res/shaders/shader.shader");
		shader.Bind();
		shader.SetUniform_4f("u_Color", { 1.f, 0.f, 0.f, 1.f });

		vertexArrayObject.UnBind();
		vertexBuffer.UnBind();
		indexBufferObject.UnBind();
		shader.UnBind();
	}
};
