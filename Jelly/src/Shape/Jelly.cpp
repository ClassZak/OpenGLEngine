#include "Jelly.hpp"

void Jelly::Init()
{
	for(auto& list : m_lines)
		for(auto& el : list)
			el.Init();
}

void Jelly::Draw()
{
	for (auto& list : m_lines)
		for (auto& el : list)
			el.Draw();
}
