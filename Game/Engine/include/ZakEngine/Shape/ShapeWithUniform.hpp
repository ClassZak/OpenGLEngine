#pragma once
#include "../OpenGLClass/Uniform.hpp"
#include "Interfaces/IDrawable.hpp"

struct ShapeWithUniform
{
	IDrawableOpenGL* m_iDrawableOpenGL;
	Uniform* m_uniform;
};