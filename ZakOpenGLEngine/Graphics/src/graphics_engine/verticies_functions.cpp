#include "../../include/graphics_engine/verticies_functions.hpp"



/* Mapping verticies for window */
void MappingVerticies(std::vector<Zak::Vertex2D<float>>& verticies, float startX, float endX, float startY, float endY, float startXDrawing, float endXDrawing, float startYDrawing, float endYDrawing)
{
	if(startX > endX)
		throw std::runtime_error("Start x cannot be more than end x");
	if(startY > endY)
		throw std::runtime_error("Start y cannot be more than end y");
	if(startXDrawing > endXDrawing)
		throw std::runtime_error("Start x for drawing cannot be more than end x for drawing");
	if(startYDrawing > endYDrawing)
		throw std::runtime_error("Start y for drawing cannot be more than end y for drawing");

	static float delta_x = std::abs(endX - startX);
	static float delta_y = std::abs(endY - startY);
	static float delta_x_drawing = std::abs(endXDrawing - startXDrawing);
	static float delta_y_drawing = std::abs(endYDrawing - startYDrawing);

	for(auto && el : verticies)
	{
		el.x = startXDrawing + ((el.x - startX) / delta_x) * delta_x_drawing;
		el.y = startYDrawing + ((el.y - startY) / delta_y) * delta_y_drawing;
	}
}


/* Full recalculation */
void RecalculateFunctionVerticies(std::vector<Zak::Vertex2D<float>>& verticies, float startX, float endX, const std::function<float(float)>& func, const Zak::Vertex2D<float>& center)
{
	if(verticies.size() <= 1)
		throw std::runtime_error("Vector is very small for recalculation function verrticies");

	static float delta = std::abs(endX - startX) / (float)(verticies.size() - 1);

	for(std::size_t i = 0; i != verticies.size() - 1; ++i)
	{
		verticies[i].x = startX + i*delta;
		verticies[i].y = func(verticies[i].x)+center.y;

		verticies[i].x += center.x;
		verticies[i].y += center.y;
	}
	verticies.back().x = endX + center.x;
	verticies.back().y = func(verticies.back().x) + center.y; 
}

