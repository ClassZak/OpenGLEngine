#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <functional>

#include "../utils/GLMacro.h"
namespace Zak {
struct VertexBufferElement
{
	unsigned int	count;
	unsigned int	type;
	unsigned char	is_normalized;

	static unsigned char GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:
				return sizeof(GLfloat);
				break;
			case GL_UNSIGNED_INT:
				return sizeof(GLuint);
				break;
			case GL_UNSIGNED_BYTE:
				return sizeof(GLubyte);
				break;
			default:
				GL_ASSERT(false);
				return 0;
		}
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride{0};
public:
	template<typename T>
	void Push(unsigned int count)
	{
#ifdef _WIN32
		static_assert(false);
#endif
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_elements.push_back({count, GL_FLOAT, GL_FALSE});
		m_stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({count, GL_UNSIGNED_INT, GL_FALSE});
		m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}
	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({count, GL_UNSIGNED_BYTE, GL_TRUE});
		m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}



	inline unsigned int GetStride() const
	{
		return m_stride;
	}
	inline const std::vector<VertexBufferElement>& GetElements() const
	{
		return m_elements;
	}
};
}