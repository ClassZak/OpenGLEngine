#include <tiny_obj_loader.h>

#include "../../include/ZakEngine/EngineCore/ObjLoader.hpp"


#include <filesystem>
#include "../../include/ZakEngine/OpenGLClass/Mesh.hpp"

using namespace Zak;
bool ObjLoader::LoadMesh
(
	const std::string& path,
	std::vector<Vertex3DNormText>& vertices,
	std::vector<unsigned int>& indices,
	bool saveUnique
)
{
	if(!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
		return EXIT_FAILURE;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
	{
		std::cerr	<< "Failed to load mesh from \"" << path << "\"" 
					<< std::endl << "Warning:\t\"" << warn	<< '\"' 
					<< std::endl << "Error:\t\t\"" << err	<< '\"' << std::endl;
	}

	static std::unordered_map<Vertex3DNormText, unsigned int> unique_vertices;
	unique_vertices.clear();

	for(auto& shape : shapes)
		for (auto& index : shape.mesh.indices)
		{
			Vertex3DNormText vertex;

			// Position
			vertex.m_pos =
			{
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2],
			};

			// Normals
			if (index.normal_index >= 0)
				vertex.m_normal =
				{
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};

			// Texture coordinates
			if(index.texcoord_index >= 0)
				vertex.m_texturePos = 
				{
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1],
				};


			if (!saveUnique)
			{
				vertices.push_back(vertex);
				indices.push_back(static_cast<unsigned int>(vertices.size()) - 1);
			}
			else
			if (unique_vertices.find(vertex) == unique_vertices.end())
			{
				unsigned int index = static_cast<unsigned int>(vertices.size());
				vertices.push_back(vertex);
				indices.push_back(index);
				unique_vertices[vertex] = index;
			}
		}

	return EXIT_SUCCESS;
}