#pragma once
#include <unordered_map>
#include <string>
#include <memory>

class Texture;
class Mesh;
class Shader;

class AssetsManager
{
private:
	static std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
	static std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshes;
	static std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;

public:
	// Текстуры
	static std::shared_ptr<Texture> LoadTexture(const std::string& name, const std::string& path);
	static std::shared_ptr<Texture> GetTexture(const std::string& name);

	// Меши
	static std::shared_ptr<Mesh> LoadMesh(const std::string& name, const std::string& path,
		std::shared_ptr<Texture> texture = nullptr);
	static std::shared_ptr<Mesh> GetMesh(const std::string& name);

	// Шейдеры
	static std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& path);
	static std::shared_ptr<Shader> GetShader(const std::string& name);

	// Очистка
	static void Clear();
};