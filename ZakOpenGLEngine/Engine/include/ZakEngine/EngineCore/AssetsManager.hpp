#pragma once
#include <unordered_map>
#include <string>
#include <memory>


namespace Zak {
class Texture;
class Mesh;
class Shader;

class AssetsManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
	std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshes;
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;

public:
	// Текстуры
	std::shared_ptr<Texture> LoadTexture(const std::string& name, const std::string& path);
	std::shared_ptr<Texture> GetTexture(const std::string& name);

	// Меши
	std::shared_ptr<Mesh> LoadMesh
	(const std::string& name, const std::string& path, std::shared_ptr<Texture> texture = nullptr);
	std::shared_ptr<Mesh> GetMesh(const std::string& name);

	// Шейдеры
	std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& path);
	std::shared_ptr<Shader> GetShader(const std::string& name);

	// Очистка
	void Clear();

	static AssetsManager& GetInstance();
};
}
