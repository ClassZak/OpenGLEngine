#pragma once
#include <vector>
#include <list>

#include "IDrawable.hpp"
#include "Line.hpp"


class Jelly : public IDrawable
{
protected:
	std::list<std::list<Line<float>>> m_lines;
public:
	Jelly()
	{
		
	}

	void Init() override;
	void Draw() override;
};





