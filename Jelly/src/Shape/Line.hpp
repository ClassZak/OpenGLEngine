#pragma once
#include "AShape.hpp"
#include "../Vertex/Vertex2D.hpp"

#include <functional>
#include <cmath>
#include <GL/glew.h>

template<typename T>
class Line : public AShape<Vertex2D<T>>
{
protected:
	GLenum m_drawMode = GL_LINE_STRIP;
	T m_lineWidth;
	bool m_isSmooth;
public:
	Line
	(
		std::initializer_list<Vertex2D<T>> vertices,
		const std::function<void(void)>& shaderUniformsProgram = nullptr,
		GLenum drawMode = GL_LINE_STRIP,
		T lineWidth = 1,
		bool isSmooth=false,
		unsigned int VAO = 0u,
		unsigned int VBO = 0u
	) : Line
	(	// Явно конвертируем initializer_list в vector для перегрузки
		std::vector<Vertex2D<T>>(vertices), 
		shaderUniformsProgram,
		drawMode,
		lineWidth,
		isSmooth,
		VAO,
		VBO
	) { }

	Line
	(
		const std::vector<Vertex2D<T>>& vertices,
		const std::function<void(void)>& shaderUniformsProgram = nullptr,
		GLenum drawMode = GL_LINE_STRIP,
		T lineWidth = 1,
		bool isSmooth = false,
		unsigned int VAO = 0u,
		unsigned int VBO = 0u
	) : 
	m_drawMode(drawMode), 
	m_lineWidth(lineWidth), 
	m_isSmooth(isSmooth)
	{
		this->m_vertexes = vertices;
		this->m_VAO = VAO;
		this->m_VBO = VBO;
		this->SetShaderUniformsProgram(shaderUniformsProgram);
	}




	Line(const Line<T>& other) :
		AShape<Vertex2D<T>>(other),
		m_drawMode(other.m_drawMode),
		m_lineWidth(other.m_lineWidth),
		m_isSmooth(other.m_isSmooth)
	{
	}
	Line& operator=(const Line& other)
	{
		if (this != &other)
		{
			AShape<Vertex2D<T>>::operator=(other);
			m_drawMode = other.m_drawMode;
			m_lineWidth = other.m_lineWidth;
			m_isSmooth = other.m_isSmooth;
		}
		return *this;
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

	glBufferData
	(
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
	// m_lineWidth != 1
	if (abs((double)m_lineWidth - 1) > 1e-10)
		glLineWidth(m_lineWidth);

	if (m_isSmooth)
	{
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	}



	GLLogCall(glBindVertexArray(this->m_VAO));
	GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO));
	glBufferData
	(
		GL_ARRAY_BUFFER,
		this->m_vertexes.size() * sizeof(Vertex2D<T>),
		this->m_vertexes.data(),
		GL_STATIC_DRAW
	);

	if (this->m_shaderUniformsProgram)
		this->m_shaderUniformsProgram();



	if (m_isSmooth)
		glDisable(GL_LINE_SMOOTH);

	GLLogCall(glDrawArrays(m_drawMode, 0, this->m_vertexes.size()));
}
