#pragma once
#include "AShape.hpp"
#include "../Vertex/Vertex2D.hpp"
#include <functional>
#include <GL/glew.h>

template<typename T>
class Line : public AShape<Vertex2D<T>>
{
protected:
	std::function<void(void)> m_shaderUniformsProgram;
	GLenum m_drawMode = GL_LINE_STRIP;

public:
	Line
	(
		std::initializer_list<Vertex2D<T>> vertices,
		const std::function<void(void)>& shaderUniformsProgram = nullptr,
		GLenum drawMode = GL_LINE_STRIP,
		unsigned int VAO = 0u,
		unsigned int VBO = 0u
	) : Line
	(	// Явно конвертируем initializer_list в vector для перегрузки
		std::vector<Vertex2D<T>>(vertices), 
		shaderUniformsProgram,
		drawMode,
		VAO,
		VBO
	) { }

	Line
	(
		const std::vector<Vertex2D<T>>& vertices,
		const std::function<void(void)>& shaderUniformsProgram = nullptr,
		GLenum drawMode = GL_LINE_STRIP,
		unsigned int VAO = 0u,
		unsigned int VBO = 0u
	) : m_drawMode(drawMode), m_shaderUniformsProgram(shaderUniformsProgram)
	{
		this->m_vertexes = vertices;
		this->m_VAO = VAO;
		this->m_VBO = VBO;
	}


	~Line()=default;

	void Init() override;
	void Draw() override;
};

template<typename T>
void Line<T>::Init()
{
	if (!this->m_VAO)
		GLLogCall(glGenVertexArrays(1, &this->m_VAO));
	GLLogCall(glBindVertexArray(this->m_VAO));

	if (!this->m_VBO)
		GLLogCall(glGenBuffers(1, &this->m_VBO));
	GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO));

	glBufferData(
		GL_ARRAY_BUFFER,
		this->m_vertexes.size() * sizeof(Vertex2D<T>),
		this->m_vertexes.data(),
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer
	(
		0,
		VERTEX_ATTRIBUTE_SIZE,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex2D<T>),
		(void*)0
	);
}

template<typename T>
void Line<T>::Draw()
{
	GLLogCall(glBindVertexArray(this->m_VAO));
	GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO));

	if (m_shaderUniformsProgram)
		m_shaderUniformsProgram();

	GLLogCall(glDrawArrays(m_drawMode, 0, this->m_vertexes.size()));
}
