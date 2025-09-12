#pragma once 
#define _USE_MATH_DEFINES

#include "AShape.hpp"
#include "../Vertex/Vertex2D.hpp"

#include <functional>
#include <stdexcept>
#include <cmath>

template<class T>
class OldQuadrangle : public AShape<Vertex2D<T>>
{
public:
	OldQuadrangle
	(
		std::initializer_list<Vertex2D<T>> vertices,
		const std::function<void(void)>& shaderUniformsProgram = nullptr,
		unsigned int VAO = 0u,
		unsigned int VBO = 0u,
		unsigned int IBO = 0u
	) : OldQuadrangle(std::vector<Vertex2D<T>>(vertices), shaderUniformsProgram, VAO, VBO, IBO)
	{
	}
	
	OldQuadrangle
	(
		const std::vector<Vertex2D<T>>& vertices,
		const std::function<void(void)>& shaderUniformsProgram = nullptr,
		unsigned int VAO = 0u,
		unsigned int VBO = 0u,
		unsigned int IBO = 0u
	)
	{
		if(vertices.size()!=4u)
			throw std::invalid_argument("Wrong size of vector");
		
		this->m_VAO = VAO;
		this->m_VBO = VBO;
		this->m_IBO = IBO;

		this->SetShaderUniformsProgram(shaderUniformsProgram);

		this->m_vertexes = vertices;
		this->m_indexes = {0, 1, 2, 0, 2, 3};
	}
	

	OldQuadrangle(const OldQuadrangle<T>& other) : AShape<Vertex2D<T>>(other)
	{
	}

	OldQuadrangle& operator=(const OldQuadrangle& other)
	{
		if (this != &other)
			AShape<Vertex2D<T>>::operator=(other);
		
		return *this;
	}


	~OldQuadrangle() = default;
	void Init() override;
	void Draw() override;
};

template<class T>
inline void OldQuadrangle<T>::Init()
{
	if (!this->m_VAO)
		GLLogCall(glGenVertexArrays(1, &this->m_VAO));
	GLLogCall(glBindVertexArray(this->m_VAO));

	if (!this->m_VBO)
		GLLogCall(glGenBuffers(1, &this->m_VBO));
	GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO));

	glBufferData
	(
		GL_ARRAY_BUFFER,
		this->m_vertexes.size() * sizeof(Vertex2D<T>),
		this->m_vertexes.data(),
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer
	(0, VERTEX_ATTRIBUTE_SIZE, GL_FLOAT, GL_FALSE, sizeof(T) * VERTEX_ATTRIBUTE_SIZE, 0);

	if (!this->m_IBO)
		glGenBuffers(1, &this->m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IBO);

	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		this->m_indexes.size() * sizeof(unsigned int),
		this->m_indexes.data(),
		GL_STATIC_DRAW
	);
}

template<class T>
inline void OldQuadrangle<T>::Draw()
{
	GLLogCall(glBindVertexArray(this->m_VAO));
	GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO));

	GLLogCall(glBufferData
	(
		GL_ARRAY_BUFFER,
		this->m_vertexes.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
		this->m_vertexes.data(),
		GL_STATIC_DRAW
	));
	GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IBO));

	if (this->m_shaderUniformsProgram)
		this->m_shaderUniformsProgram();

	GLLogCall(glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		this->m_indexes.size() * sizeof(std::vector<unsigned int>::value_type),
		this->m_indexes.data(),
		GL_STATIC_DRAW
	));

	GLLogCall(glDrawElements(GL_TRIANGLES, this->m_indexes.size(), GL_UNSIGNED_INT, nullptr));
}