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




	unsigned int VAO;
	GLLogCall(glGenVertexArrays(1, &VAO));
	GLLogCall(glBindVertexArray(VAO));




	const std::vector<Vertex2D<float>> vertexes =
	{
		Vertex2D(-0.5f		,	-0.5f),
		Vertex2D(-0.5f		,	0.5f),
		Vertex2D(0.5f		,	0.5f),

		Vertex2D(-0.5f		,	-0.5f),
		Vertex2D(0.5f		,	0.5f),
		Vertex2D(0.5f		,	-0.5f),
	};
	std::vector<Vertex2D<float>> vertexBufferData = GetUniqueVertexes(vertexes);
	std::vector<unsigned int> vertexesIndices =
	{
		0,	1,	2,
		0,	2,	3
	};




	unsigned int VBO{};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		vertexBufferData.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
		vertexBufferData.data(),
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



	GLuint shaderProgram = Shader::CreateShader(shader.m_vertexShader, shader.m_fragmentShader);
	glUseProgram(shaderProgram);


	int location = glGetUniformLocation(shaderProgram, "u_Color");
	GL_ASSERT(location != -1);
	GLLogCall(glUniform4f(location, 0.2f, 0.2f, 02.f, 1.f));

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
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

		glDrawElements(GL_TRIANGLES, vertexesIndices.size(), GL_UNSIGNED_INT, nullptr);
		//glDrawArrays(GL_TRIANGLES, NULL, vertexBufferData.size());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		now = std::chrono::system_clock::now();
	}

	glDeleteProgram(shaderProgram);

	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();

	return 0;
}