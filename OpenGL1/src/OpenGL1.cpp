#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <locale>
#include <chrono>
#include <cmath>
#include <vector>

#include <conio.h>
#include <stdlib.h>

#define FPS 60.


int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");

	std::cout << "Hello CMake." << std::endl;

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();



	GLFWwindow* window;



	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return -1;

	unsigned int buffer{};

	std::vector<float> vertexes = {
		0	,	1,
		-1	,	-1,
		0.99,	-1,
		0	,	1,
		1	,	1,
		1	,	-1
	};

	float bufferData[12];
	for(std::size_t i = 0; i!=vertexes.size();++i)
		bufferData[i] = vertexes[i];

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		std::chrono::system_clock::duration duration_since_epoch = now.time_since_epoch();
		std::chrono::milliseconds	milliseconds_since_epoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);
		std::chrono::seconds		seconds_since_epoch =
		std::chrono::duration_cast<std::chrono::seconds>(milliseconds_since_epoch);

		long diff = 2L;
		bufferData[0] = sin(milliseconds_since_epoch.count() / 1000.) * (bufferData[2*2] - vertexes[0]) + 0.;

		while (1000. / diff >= FPS)
			diff = std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now() - now).count();

		{
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
		}
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);



		glDrawArrays(GL_TRIANGLES, NULL, sizeof(bufferData) / sizeof(*bufferData));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		now = std::chrono::system_clock::now();
	}

	glfwTerminate();
	return 0;
}
