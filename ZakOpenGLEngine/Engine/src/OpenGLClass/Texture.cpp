#include "../../include/ZakEngine/OpenGLClass/Texture.hpp"
#include <filesystem>
#include <memory>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <tiny_obj_loader.h>

#include <stb_image.h>


#include "../../include/ZakEngine/utils/GLMacro.h"

using namespace Zak;

Texture::Texture(const std::string& filename)
{
	m_filename = new char[filename.size() + 1];
	strcpy(m_filename, filename.c_str());

	if (!std::filesystem::exists(m_filename))
		throw std::runtime_error(std::string("Not found \"") + m_filename + "\"");
	stbi_set_flip_vertically_on_load(1);
	m_data = stbi_load(m_filename, &m_width, &m_height, &m_BPP, 4);

	glGenTextures(1, &m_index);
	glBindTexture(GL_TEXTURE_2D, m_index);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	GLLogCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data));

	UnBind();
}
Texture::~Texture()
{
	if(m_filename)
		delete [] m_filename;
	if(m_data)
		stbi_image_free(m_data);

	if(m_index)
		glDeleteTextures(1, &m_index);
}




void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_index);
}
Texture::Texture(unsigned int id, const char* filename, unsigned char* data, int width, int height, int BBP) :
m_width(width), m_height(height), m_BPP(BBP)
{
	m_index = id;
	if (filename)
	{
		m_filename = new char[strlen(filename) + 1];
		strcpy(m_filename, filename);
	}
	if (!data)
		throw std::runtime_error("Unable to copy empty texture data");

	size_t full_size = m_width * m_height;
	m_data = new unsigned char[full_size];
	memccpy(m_data, data, '\0', full_size);

}
void Texture::Bind(unsigned char slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	Bind();
}
void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::shared_ptr<Texture> Texture::CreateTestTexture()
{
	// Создаем текстуру 16x16
	const int size = 16;
	std::vector<unsigned char> pixels = GenerateTextureData_GradientPattern(size);

	// Создаем временную текстуру
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Создаем объект Texture (может потребоваться адаптация вашего класса)
	Texture* texture = new Texture(texture_id, nullptr, pixels.data(), size, size, 4);
	std::shared_ptr<Texture> texture_shared_ptr(texture);

	return texture_shared_ptr;
}

std::vector<unsigned char> Texture::GenerateTextureData_CheckerboardPattern(size_t size)
{
	std::vector<unsigned char> pixels(size * size * 4);
	for (int y = 0; y < size; ++y)
	{
		for (int x = 0; x < size; ++x)
		{
			int index = (y * size + x) * 4;
			bool isBlack = (x / 4 + y / 4) % 2 == 0;

			pixels[index + 0] = isBlack ? 0 : 255;	// R
			pixels[index + 1] = isBlack ? 0 : 255;	// G
			pixels[index + 2] = isBlack ? 0 : 255;	// B
			pixels[index + 3] = 255;				// A
		}
	}
	
	return pixels;
}

std::vector<unsigned char> Texture::GenerateTextureData_GradientPattern(size_t size)
{
	std::vector<unsigned char> pixels(size * size * 4);
	for(int y = 0; y != size; ++y)
		for (int x = 0; x != size; ++x)
		{
			int index = (y * size + x) * 4;

			pixels[index + 0] = 255;
			pixels[index + 1] = x ? x * 255 / size : 0;
			pixels[index + 2] = y ? y * 255 / size : 0;
			pixels[index + 3] = 255;
		}


	return pixels;
}