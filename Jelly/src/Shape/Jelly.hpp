#pragma once
#include "IDrawable.hpp"
#include "Line.hpp"

#include <vector>
#include <list>


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





