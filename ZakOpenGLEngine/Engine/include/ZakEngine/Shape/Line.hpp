#pragma once
#include "../OpenGLClass/Shader.hpp"
#include "../Shape/Interfaces/IHasIndexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexBufferObject.hpp"
#include "../Shape/Interfaces/IHasVertexArrayObject.hpp"
#include "../Shape/Interfaces/IHasShader.hpp"
#include "../Shape/Interfaces/IDrawable.hpp"

#include "../Shape/Interfaces/IHasVertexVector.hpp"
#include "../Vertex/Vertex2D.hpp"

#include <functional>
#include <cmath>
#include <GL/glew.h>

namespace Zak {
template<typename T>
class Line :
	public IHasVertexVector<Vertex2D<T>>,
	public IHasShader
{
protected:
	double m_lineWidth;
	bool m_isSmooth;
public:
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
		::Zak::IHasVertexBufferObject::m_drawMode = drawMode;
		::Zak::IHasVertexVector<Vertex2D<T>>::Init(vertices);
		Init(vertices);
	}

	Line(const Line& other) :
	IHasVertexVector<Vertex2D<T>>(other),
	m_lineWidth(other.m_lineWidth),
	m_isSmooth(other.m_isSmooth)
	{
		::Zak::IHasVertexBufferObject::m_drawMode = other.GetDrawMode();
		::Zak::IHasVertexVector<Vertex2D<T>>::Init(other.m_vertices);
		Init(this->m_vertices);
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


protected:
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
}