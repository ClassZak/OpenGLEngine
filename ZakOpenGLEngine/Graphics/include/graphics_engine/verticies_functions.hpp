#include <vector>
#include <functional>
#include <ZakEngine/Vertex/Vertex2D.hpp>




void MappingVerticies(std::vector<Zak::Vertex2D<float>>& verticies, float startX, float endX, float startY, float endY, float startXDrawing, float endXDrawing, float startYDrawing, float endYDrawing);
void RecalculateFunctionVerticies(std::vector<Zak::Vertex2D<float>>& verticies, float startX, float endX, const std::function<float(float)>& func, const Zak::Vertex2D<float>& center = {0.f, 0.f});

