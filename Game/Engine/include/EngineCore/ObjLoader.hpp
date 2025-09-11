#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>

#include "../Vertex/Vertex3DNormText.hpp"


class ObjLoader
{
public:
	static bool LoadMesh
	(
		const std::string& path,
		std::vector<Vertex3DNormText>& vertices,
		std::vector<unsigned int>& indices, 
		bool saveUnique = false
	);
};