#pragma once

class IDrawable
{
	virtual void Draw() = 0;
	virtual void Init() = 0;
};

class IDrawableOpenGL
{
protected:
	IDrawableOpenGL() = default;
public:
	virtual ~IDrawableOpenGL()
	{} // Needs a virtual destructor for dynamic_cast to work
};

