#include "Texture.hpp"
#include <filesystem>

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


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




void Texture::Load()
{
	if(!std::filesystem::exists(m_filename))
		throw std::runtime_error(std::string("Not found \"") + m_filename + "\"");
	stbi_set_flip_vertically_on_load(1);
	m_data = stbi_load(m_filename, &m_width, &m_height, &m_BPP, 4);
}
void Texture::Init()
{
	glGenTextures(1, &m_index);
	glBindTexture(GL_TEXTURE_2D, m_index);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	stbi_image_free(m_data);

	GLLogCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_INT, m_data));
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

