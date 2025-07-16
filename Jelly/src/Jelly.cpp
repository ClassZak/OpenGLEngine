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
static inline std::vector<Vertex2D<T>> GenerateCircleVertexes
(std::size_t count, T radius, const Vertex2D<T>& center)
{
	std::vector<Vertex2D<T>> vertexes;
	vertexes.reserve(count);

	double sector = 2 * M_PI / count;

	for (std::size_t i = 0; i < count; ++i)
	{
		T x = static_cast<T>(cos(sector * i)*radius), y = static_cast<T>(sin(sector * i)* radius);
		vertexes.emplace_back(x + center.x, y + center.y);
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




	unsigned int VAO1;
	GLLogCall(glGenVertexArrays(1, &VAO1));
	GLLogCall(glBindVertexArray(VAO1));




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




	unsigned int VBO1{};
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		vertexBufferData.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
		vertexBufferData.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VERTEX_ATTRIBUTE_SIZE, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_ATTRIBUTE_SIZE, 0);

	unsigned int IBO1{};
	glGenBuffers(1, &IBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		vertexesIndices.size() * sizeof(std::vector<unsigned int>::value_type),
		vertexesIndices.data(),
		GL_STATIC_DRAW
	);




	
	unsigned int VAO2;
	GLLogCall(glGenVertexArrays(1, &VAO2));
	GLLogCall(glBindVertexArray(VAO2));

	const unsigned int circlePointCount = 50;
	float radius = 0.1f;
	const Vertex2D<float> center(0.75f, 0.f);
	std::vector<Vertex2D<float>> vertexes2 = GenerateCircleVertexes(circlePointCount, radius, center);
	std::vector<Vertex2D<float>> vertexBufferData2;
	vertexBufferData2.push_back(center);
	vertexBufferData2.insert(vertexBufferData2.end(), vertexes2.begin(), vertexes2.end());

	// Генерация индексов для треугольников
	std::vector<unsigned int> vertexesIndices2;
	for (int i = 0; i < circlePointCount -1 ; i++)
	{
		vertexesIndices2.push_back(0); // Центральная вершина
		vertexesIndices2.push_back(1 + i); // Текущая вершина окружности
		vertexesIndices2.push_back(1 + (i + 1) % circlePointCount); // Следующая вершина окружности
	}

	for(auto& el : vertexesIndices2)
		std::cout<<el<<"\n";
	std::cout<<std::endl;
	std::cout<<vertexesIndices2.size()<<std::endl;
	std::cout<<std::endl<<std::endl;
	for(auto& el : vertexBufferData2)
		printf("(%f\t,%f)\n",el.x,el.y);
	std::cout<<std::endl;
	std::cout<<vertexBufferData2.size()<<std::endl;
		

	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		vertexBufferData2.size() * sizeof(Vertex2D<float>),
		vertexBufferData2.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VERTEX_ATTRIBUTE_SIZE, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_ATTRIBUTE_SIZE, 0);

	unsigned int IBO2;
	glGenBuffers(1, &IBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO2);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		vertexesIndices2.size() * sizeof(unsigned int),
		vertexesIndices2.data(),
		GL_STATIC_DRAW
	);





	GLuint shaderProgram = Shader::CreateShader(shader.m_vertexShader, shader.m_fragmentShader);
	glUseProgram(shaderProgram);


	int location = glGetUniformLocation(shaderProgram, "u_Color");
	GL_ASSERT(location != -1);
	GLLogCall(glUniform4f(location, 0.2f, 0.2f, 02.f, 1.f));

	
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




		double sector = 
		(2 * M_PI) * 
		fmod((double)millisecondsSinceEpoch.count(),1000) / 1000
		/ 
		(circlePointCount-1)
		;

		for (std::size_t i = 1; i < vertexBufferData2.size(); ++i)
		{
			vertexBufferData2[i].x = 
			center.x + cos(sector * (i-1)) * radius;
			vertexBufferData2[i].y = 
			center.y + sin(sector * (i-1)) * radius;
		}

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
		
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);




		glBindVertexArray(VAO1);
		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData
		(
			GL_ARRAY_BUFFER,
			vertexBufferData.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
			vertexBufferData.data(),
			GL_STATIC_DRAW
		);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
		glBufferData
		(
			GL_ELEMENT_ARRAY_BUFFER,
			vertexesIndices.size() * sizeof(std::vector<unsigned int>::value_type) * vertexesIndices.size(),
			vertexesIndices.data(),
			GL_STATIC_DRAW
		);
		glDrawElements(GL_TRIANGLES, vertexesIndices.size(), GL_UNSIGNED_INT, nullptr);






		glBindVertexArray(VAO2);
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData
		(
			GL_ARRAY_BUFFER,
			vertexBufferData2.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
			vertexBufferData2.data(),
			GL_STATIC_DRAW
		);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO2);
		glUniform4f(location, 1.0f, 0.5f, 1.0f, 1.0f);
		glBufferData
		(
			GL_ELEMENT_ARRAY_BUFFER,
			vertexesIndices2.size() * sizeof(std::vector<unsigned int>::value_type)* vertexesIndices.size(),
			vertexesIndices2.data(),
			GL_STATIC_DRAW
		);
		glDrawElements(GL_TRIANGLES, vertexesIndices2.size(), GL_UNSIGNED_INT, nullptr);



		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		now = std::chrono::system_clock::now();
	}

	glDeleteProgram(shaderProgram);

	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);

	glfwTerminate();

	return 0;
}