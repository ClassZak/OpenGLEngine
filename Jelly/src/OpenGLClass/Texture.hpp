#pragma once
#include <string>

#include "AOpenGLClass.hpp"



class Texture : AOpenGLClass
{
protected:
	char* m_filename = nullptr;
	unsigned char* m_data = nullptr;

	int m_width = 0, m_height = 0, m_BPP = 0;

	void Bind() const override;
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
};
