#include "Renderer.hpp"

Renderer& Renderer::GetInstance()
{
	//Singleton �� C++11
	static Renderer instance;
	return instance;
}



