#include "AssetsManager.hpp"
#include "ObjLoader.hpp"

#include "../OpenGLClass/Texture.hpp"
#include "../OpenGLClass/Mesh.hpp"
#include "../OpenGLClass/Shader.hpp"

#include <filesystem>


std::shared_ptr<Texture> AssetsManager::LoadTexture(const std::string& name, const std::string& path)
{
	if(!std::filesystem::exists(path))
		throw std::runtime_error(std::string("Not found \"") + path + "\"");
	if(!std::filesystem::is_regular_file(path))
		throw std::runtime_error(std::string("\"") + path + "\" is not regular file");

	auto it = m_textures.find(name);
	if(it != m_textures.end())
		return it->second;
	
	auto texture = std::make_shared<Texture>(path);
	m_textures[name] = texture;

	return texture;
}
std::shared_ptr<Texture> AssetsManager::GetTexture(const std::string& name)
{
	auto it = m_textures.find(name);
	return it == m_textures.end() ? nullptr : it->second;
}


std::shared_ptr<Mesh> AssetsManager::LoadMesh
(const std::string& name, const std::string& path, std::shared_ptr<Texture> texture)
{
	if (!std::filesystem::exists(path))
		throw std::runtime_error(std::string("Not found \"") + path + "\"");
	if (!std::filesystem::is_regular_file(path))
		throw std::runtime_error(std::string("\"") + path + "\" is not regular file");

	auto it = m_meshes.find(name);
	if (it != m_meshes.end())
		return it->second;

	std::vector<Vertex3DNormText> vertices;
	std::vector<unsigned int> indices;

	if (ObjLoader::LoadMesh(path, vertices, indices))
		return nullptr;

	auto mesh = std::make_shared<Mesh>(vertices, indices, texture, nullptr);
	m_meshes[name] = mesh;

	return mesh;
}
std::shared_ptr<Mesh> AssetsManager::GetMesh(const std::string& name)
{
	auto it = m_meshes.find(name);
	return it == m_meshes.end() ? nullptr : it->second;
}


std::shared_ptr<Shader> AssetsManager::LoadShader(const std::string& name, const std::string& path)
{
	if (!std::filesystem::exists(path))
		throw std::runtime_error(std::string("Not found \"") + path + "\"");
	if (!std::filesystem::is_regular_file(path))
		throw std::runtime_error(std::string("\"") + path + "\" is not regular file");

	auto it = m_shaders.find(name);
	if(it != m_shaders.end())
		return it->second;

	auto shader = std::make_shared<Shader>(path);
	m_shaders[name] = shader;

	return shader;
}
std::shared_ptr<Shader> AssetsManager::GetShader(const std::string& name)
{
	auto it = m_shaders.find(name);
	return it == m_shaders.end() ? nullptr : it->second;
}


void AssetsManager::Clear()
{
	m_textures.clear();
	m_meshes.clear();
	m_shaders.clear();
}




AssetsManager& AssetsManager::GetInstance()
{
	//Singleton из C++11
	static AssetsManager assets_manager;
	return assets_manager;
}

