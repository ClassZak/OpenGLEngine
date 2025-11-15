#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <tiny_obj_loader.h>
#include <stb_image.h>

#include <iostream>
#include <locale>
#include <stdlib.h>
#include <mutex>
#include <atomic>

#include <stdio.h>
#include <unistd.h>
#include <errno.h>


#include <ZakEngine/EngineCore/Renderer.hpp>
#include <ZakEngine/EngineCore/AssetsManager.hpp>
#include <ZakEngine/Shape/Line.hpp>
#include <ZakEngine/Shape/Quadrangle.hpp>
#include <ZakEngine/Shape/CircleSector.hpp>

#include <ZakEngine/OpenGLClass/VertexArrayObject.hpp>
#include <ZakEngine/OpenGLClass/VertexBufferObject.hpp>
#include <ZakEngine/OpenGLClass/VertexBufferLayout.hpp>
#include <ZakEngine/OpenGLClass/IndexBufferObject.hpp>

#include <chrono>
#include <cmath>

float windowWidth = 640;
float windowHeight = 480;
inline void exit_failure(int code = EXIT_FAILURE);

void MappingVerticies(std::vector<Zak::Vertex2D<float>>& verticies, float startX, float endX, float startY, float endY, float startXDrawing, float endXDrawing, float startYDrawing, float endYDrawing)
{
	if(startX > endX)
		throw std::runtime_error("Start x cannot be more than end x");
	if(startY > endY)
		throw std::runtime_error("Start y cannot be more than end y");
	if(startXDrawing > endXDrawing)
		throw std::runtime_error("Start x for drawing cannot be more than end x for drawing");
	if(startYDrawing > endYDrawing)
		throw std::runtime_error("Start y for drawing cannot be more than end y for drawing");

	static float delta_x = startX - endX;
	static float delta_y = startY - endY;
	static float delta_x_drawing = startXDrawing - endXDrawing;
	static float delta_y_drawing = startYDrawing - endYDrawing;

	for(auto && el : verticies)
	{
		el.x = startXDrawing + ((el.x - startX) / delta_x) * delta_x_drawing;
		el.y = startYDrawing + ((el.y - startY) / delta_y) * delta_y_drawing;
	}
}

std::vector<Zak::Vertex2D<float>> GenerateGrephicsVerticies(float startX, float  endX, float  startY, float  endY, unsigned int steps, Zak::Vertex2D<float> center, std::function<float(float)> func)
{
	if(startX > endX)
		throw std::runtime_error("Start x cannot be more than end x");
	if(startY > endY)
		throw std::runtime_error("Start y cannot be more than end y");

	std::vector<Zak::Vertex2D<float>> result(steps);
	
	static float delta = (endX - startX) / steps;
	for(unsigned int i = 0; i != steps; ++i)
	{
		result[i].x = startX + i * delta;
		result[i].y = func(result[i].x);
	}


	return result;
}


int main(int argc, char** argv)
{
#pragma region Initialization
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	GLFWwindow* window;
	if(!glfwInit())
		exit_failure();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	const char* XDG_CURRENT_DESKTOP = getenv("XDG_CURRENT_DESKTOP");
	if (XDG_CURRENT_DESKTOP && strcmp(XDG_CURRENT_DESKTOP, "GNOME") == 0)
		glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
	else
		glfwInitHint(GLFW_PLATFORM, GLFW_ANY_PLATFORM);

	window = glfwCreateWindow(windowWidth, windowHeight, "Jelly", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLFWimage images[1];
	images[0].pixels = stbi_load("../../Jelly/assets/icon.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		exit_failure();
	
#ifdef _DEBUG
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
#endif
#pragma region Fallback functions
	glfwSetFramebufferSizeCallback
	(
		window,
		[](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
			windowHeight = height;
			windowWidth = width;
		}
	);
#pragma endregion
#pragma endregion 
#pragma region AssetsManager
	std::shared_ptr<Zak::Shader> default_shader;
	std::shared_ptr<Zak::Shader> graphic_shader;
	try
	{
		graphic_shader	= Zak::AssetsManager::GetInstance().LoadShader
		("graphic_shader", "../../Graphics/res/shaders/graphic_shader.shader");
		default_shader	= Zak::AssetsManager::GetInstance().LoadShader
		("default_shader", "../../Graphics/res/shaders/default_shader.shader");
	}
	catch (std::exception& e)
	{
		std::cerr<<e.what()<<std::endl;
		exit_failure();
	}
#pragma endregion

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::system_clock::duration duration_since_epoch = now.time_since_epoch();
	std::chrono::milliseconds	milliseconds_since_epoch =
	std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);

	float a=0, b=0;
	float x0 = 0, xn = 40;
	float h = 10;
	const int VERTICES_COUNT=5;

	std::vector<Zak::Vertex2D_float> graphic_verticies(VERTICES_COUNT);
	for(std::size_t i=0; i!=graphic_verticies.size(); ++i)
		graphic_verticies[i].x = -1 + (x0 + i*h) / xn * 2;
	
	graphic_verticies[0].y = -1 + 0.f	/ 50.f * 2;
	graphic_verticies[1].y = -1 + 1.f	/ 50.f * 2;
	graphic_verticies[2].y = -1 + 2.f	/ 50.f * 2;
	graphic_verticies[3].y = -1 + 10.f	/ 50.f * 2;
	graphic_verticies[4].y = -1 + 49.f	/ 50.f * 2;

	graphic_verticies.push_back({0.f,0.f});
	graphic_verticies.push_back({0.5f,0.5f});
	
	Zak::VertexArrayObject vertexArrayObject;
	Zak::VertexBufferObject vertexBufferObject(graphic_verticies);
	Zak::VertexBufferLayout layout;
	layout.Push<float>(2);
	vertexArrayObject.AddBuffer(vertexBufferObject, layout);
	vertexArrayObject.UnBind();
	vertexBufferObject.UnBind();

	Zak::Line line(graphic_verticies);
	line.SetShader(default_shader);

while (!glfwWindowShouldClose(window))
	{
		duration_since_epoch = now.time_since_epoch();
		milliseconds_since_epoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);

#pragma region Animation
#pragma endregion

#pragma region Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.7,0.7,0.7,1.0);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);


		// ENABLE SHADER BEFORE DRAW ANYTHING !1!!!
		default_shader->Bind();
		default_shader->SetUniform
		(Zak::Uniform("u_Color", Zak::UniformVec4(1.f, abs(sin(milliseconds_since_epoch.count())), 0.f, 1.f)));
		vertexArrayObject.Bind();
		vertexBufferObject.Bind();
		glDrawArrays(GL_LINE_STRIP, 0, vertexBufferObject.GetCount());
		vertexArrayObject.UnBind();
		vertexBufferObject.UnBind();
		default_shader->UnBind();
#pragma endregion

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		now = std::chrono::system_clock::now();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}

inline void exit_failure(int code)
{
	glfwTerminate();
	exit(code);
}
