#include "Texture.hpp"
#include <filesystem>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <tiny_obj_loader.h>

#include <stb_image.h>


#include "../utils/GLMacro.h"


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
	// Создаем текстуру 16x16 с шахматным pattern
	const int size = 16;
	std::vector<unsigned char> pixels(size * size * 4);

	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			int index = (y * size + x) * 4;
			bool isBlack = (x / 4 + y / 4) % 2 == 0;

			pixels[index] = isBlack ? 0 : 255;     // R
			pixels[index + 1] = isBlack ? 0 : 255; // G
			pixels[index + 2] = isBlack ? 0 : 255; // B
			pixels[index + 3] = 255;               // A
		}
	}

	// Создаем временную текстуру
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Создаем объект Texture (может потребоваться адаптация вашего класса)
	auto texture = std::make_shared<Texture>();
	texture->m_index = textureID;
	texture->m_width = size;
	texture->m_height = size;

	return texture;
}

