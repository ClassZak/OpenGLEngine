#define _USE_MATH_DEFINES

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>

#include <iostream>
#include <locale>
#include <chrono>
#include <cmath>
#include <vector>
#include <array>
#include <algorithm>

#include <conio.h>
#include <stdlib.h>

#include <thread>
#include <utility>



#include <math.h>



#include "utils/GLMacro.h"
#include "Vertex/2DVertex.hpp"
#include "Vertex/VertexUtils.hpp"
#include "Shader/Shader.hpp"




#define FPS 60
#define VERTEX_ATTRIBUTE_SIZE 2

#define ANIMATION_SPEED 2.5e-3

/// <summary>
/// 
/// </summary>
/// <typeparam name="T">скалярный тип</typeparam>
/// <param name="count"></param>
/// <param name="center"></param>
/// <returns></returns>
template<typename T>
static inline std::vector<Vertex2D<T>> GenerateCircleVertexes(std::size_t count, const Vertex2D<T>& center)
{
	std::vector<Vertex2D<T>> vertexes;
	vertexes.reserve(count);

	double sector = M_PI * 2. / count;

	for (std::size_t i = 0; i != count; ++i)
	{
		T x=cos(sector * i), y = sin(sector * i);
		vertexes.emplace_back(Vertex2D<T>(x+center.x,y+center.y));
	}

	return vertexes;
}

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();


	std::string shaderData = LoadDataFromFile("../res/shaders/shader.shader");
	Shader shader = Shader::GetVertexAndFragmentShaders(shaderData);



	GLFWwindow* window;



	/* Initialize the library */
	if (!glfwInit())
		return -1;


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Jelly", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback
	(
		window,
		[](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	);
#if FPS <= 1000
	glfwSwapInterval(1);
#endif

	if (glewInit() != GLEW_OK)
		return -1;



	
	unsigned int vertexArrayObject;
	GLLogCall(glGenVertexArrays(1, &vertexArrayObject));
	GLLogCall(glBindVertexArray(vertexArrayObject));

	unsigned int vertexArrayObjectOfCircle;
	GLLogCall(glGenVertexArrays(1, &vertexArrayObjectOfCircle));
	GLLogCall(glBindVertexArray(vertexArrayObjectOfCircle));


	unsigned int vertexBufferObject{};
	glGenBuffers(1, &vertexBufferObject);

	unsigned int vertexBufferObjectOfCircle{};
	glGenBuffers(1, &vertexBufferObjectOfCircle);


	const std::vector<Vertex2D<float>> vertexes =
	{
		Vertex2D (-0.5f		,	-0.5f),
		Vertex2D (-0.5f		,	0.5f),
		Vertex2D (0.5f		,	0.5f),

		Vertex2D (-0.5f		,	-0.5f),
		Vertex2D (0.5f		,	0.5f),
		Vertex2D (0.5f		,	-0.5f),
	};
	std::vector<Vertex2D<float>> vertexBufferData = GetUniqueVertexes(vertexes);
	std::vector<unsigned int> vertexesIndices =
	{
		0,	1,	2,
		0,	2,	3
	};


	const std::vector<Vertex2D<float>> circleVertexes = GenerateCircleVertexes(33u, Vertex2D(0.f,0.f));
	std::vector<Vertex2D<float>> circleVertexBufferData = circleVertexes;
	std::vector<unsigned int> circleVertexesIndices = [&circleVertexes]() -> std::vector<unsigned int>
		{
			std::vector<unsigned int> indexes;
			indexes.reserve(circleVertexes.size());
			for(std::size_t i = 0; i!= circleVertexes.size(); ++i)
				indexes.emplace_back(i);

			return indexes;
		}();

	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData
	(
		GL_ARRAY_BUFFER, 
		vertexBufferData.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
		vertexBufferData.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VERTEX_ATTRIBUTE_SIZE, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_ATTRIBUTE_SIZE, 0);


	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectOfCircle);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		circleVertexBufferData.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
		circleVertexBufferData.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VERTEX_ATTRIBUTE_SIZE, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_ATTRIBUTE_SIZE, 0);


	unsigned int indexBufferObject{};
	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		vertexesIndices.size() * sizeof(std::vector<unsigned int>::value_type),
		vertexesIndices.data(),
		GL_STATIC_DRAW 
	);

	unsigned int indexBufferObjectOfCircle{};
	glGenBuffers(1, &indexBufferObjectOfCircle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjectOfCircle);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		circleVertexBufferData.size() * sizeof(std::vector<unsigned int>::value_type),
		circleVertexBufferData.data(),
		GL_STATIC_DRAW 
	);



	GLuint shaderProgram = Shader::CreateShader(shader.m_vertexShader, shader.m_fragmentShader);
	glUseProgram(shaderProgram);


	int location = glGetUniformLocation(shaderProgram, "u_Color");
	GL_ASSERT(location != -1);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		std::chrono::system_clock::duration durationSinceEpoch = now.time_since_epoch();
		std::chrono::milliseconds	millisecondsSinceEpoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(durationSinceEpoch);
		std::chrono::seconds		secondsSinceEpoch =
		std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch);


		vertexBufferData[1].x = 
		vertexes[1].x + (cos(millisecondsSinceEpoch.count() * ANIMATION_SPEED) + 1) / 2. - 0.5;
		vertexBufferData[1].y = 
		vertexes[1].y + (sin(millisecondsSinceEpoch.count() * ANIMATION_SPEED) + 1) / 2. - 0.5;






		long diff = 2L;
#if FPS <= 1000
		while (int(1000. / diff) >= FPS)
		{
			diff = std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::system_clock::now() - now).count();
		}
#endif  // FPS <= 1000
		
		glUniform4f
		(
			location,
			(sin(millisecondsSinceEpoch.count() * ANIMATION_SPEED) + 1) / 2.f,
			(cos(millisecondsSinceEpoch.count() * ANIMATION_SPEED) + 1) / 2.f,
			(tan(millisecondsSinceEpoch.count() * ANIMATION_SPEED) + 1),
			1.f
		);
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
			glBufferData
			(
				GL_ARRAY_BUFFER,
				vertexBufferData.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
				vertexBufferData.data(),
				GL_STATIC_DRAW
			);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

			
			glBufferData
			(
				GL_ELEMENT_ARRAY_BUFFER,
				vertexesIndices.size() * sizeof(std::vector<unsigned int>::value_type) * vertexesIndices.size(),
				vertexesIndices.data(),
				GL_STATIC_DRAW
			);
		}
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		glBindVertexArray(vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, vertexesIndices.size(), GL_UNSIGNED_INT, vertexesIndices.data());
		glBindVertexArray(vertexArrayObjectOfCircle);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, circleVertexesIndices.size(), GL_UNSIGNED_INT, circleVertexesIndices.data());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		now = std::chrono::system_clock::now();
	}

	glDeleteProgram(shaderProgram);

	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteVertexArrays(1, &vertexArrayObjectOfCircle);

	glfwTerminate();

	return 0;
}
