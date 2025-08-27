#pragma once
#include <string>
#include <vector>

#include "AOpenGLClass.hpp"

namespace std 
{
	_EXPORT_STD template <class _Ty>
		class shared_ptr;
}


class Texture : AOpenGLClass
{
protected:
	char* m_filename = nullptr;
	unsigned char* m_data = nullptr;

	int m_width = 0, m_height = 0, m_BPP = 0;

	void Bind() const override;
	Texture(unsigned int id, const char* filename, unsigned char* data, int width = 0, int height = 0, int BBP = 0);
public:
	Texture(const std::string& filename);
	~Texture();

	void Bind(unsigned char slot);
	void UnBind() const override;

	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }

	bool operator==(const Texture& other) const
	{
		return m_index == other.m_index;
	}

	static std::shared_ptr<Texture> CreateTestTexture();
	static std::vector<unsigned char> GenerateTextureData_CheckerboardPattern(size_t size);
	static std::vector<unsigned char> GenerateTextureData_GradientPattern(size_t size);
};
