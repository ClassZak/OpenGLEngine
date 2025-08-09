#pragma once
#include "../OpenGLClass/Shader.hpp"
#include "../Shape/Interfaces/IHasIndexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexArrayObject.hpp"
#include "../Shape/Interfaces/IHasShader.hpp"
#include "../Shape/Interfaces/IDrawable.hpp"
#include "../Vertex/Vertex2D.hpp"

#include <functional>
#include <cmath>
#include <GL/glew.h>


class Line :
	public IHasVertexArrayObject,
	public IHasVertexBufferObject,
	public IHasShader
{
protected:
	GLenum m_drawMode = GL_LINE_STRIP;
	double m_lineWidth;
	bool m_isSmooth;

	template<typename T>
	void Init(const std::vector<Vertex2D<T>>& vertices)
	{
		VertexBufferObject* newVertexBufferObject = new VertexBufferObject(vertices);
		this->m_vertexBufferObject.reset(newVertexBufferObject);
		// Attribute crafting
		VertexBufferLayout layout;
		layout.Push<T>(2);
		this->m_vertexArrayObject.AddBuffer(*this->m_vertexBufferObject.get(), layout);


		this->m_vertexArrayObject.UnBind();
		this->m_vertexBufferObject.get()->UnBind();
	}
public:
	template<typename T>
	Line
	(
		std::initializer_list<Vertex2D<T>> vertices,
		GLenum drawMode = GL_LINE_STRIP,
		double lineWidth = 1,
		bool isSmooth=false
	) : Line
	(	// Явно конвертируем initializer_list в vector для перегрузки
		std::vector<Vertex2D<T>>(vertices),
		drawMode,
		lineWidth,
		isSmooth
	) { }

	template<typename T>
	Line
	(
		const std::vector<Vertex2D<T>>& vertices,
		GLenum drawMode = GL_LINE_STRIP,
		double lineWidth = 1,
		bool isSmooth = false
	) : 
	m_lineWidth(lineWidth), 
	m_isSmooth(isSmooth)
	{
		::IHasVertexBufferObject::m_drawMode = drawMode;
		Init(vertices);
	}




	Line(const Line& other) :
		m_lineWidth(other.m_lineWidth),
		m_isSmooth(other.m_isSmooth)
	{
		this->m_drawMode=other.m_drawMode;
	}
	Line& operator=(const Line& other)
	{
		if (this != &other)
		{
			this->m_drawMode = other.m_drawMode;
			m_lineWidth = other.m_lineWidth;
			m_isSmooth = other.m_isSmooth;
		}
		return *this;
	}

	~Line()=default;

	double GetWidth() const
	{
		return m_lineWidth;
	}
	bool IsSmooth() const
	{
		return m_isSmooth;
	}
};

