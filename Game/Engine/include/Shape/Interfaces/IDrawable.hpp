#pragma once
#include <GL/glew.h>

class IDrawable
{
	virtual void Draw() = 0;
	virtual void Init() = 0;
};

class IDrawableOpenGL
{
protected:
	IDrawableOpenGL() = default;
	GLenum m_drawMode = GL_TRIANGLES;
public:
	virtual ~IDrawableOpenGL()
	{} // Needs a virtual destructor for dynamic_cast to work
	GLenum GetDrawMode() const
	{
		return m_drawMode;
	}
};