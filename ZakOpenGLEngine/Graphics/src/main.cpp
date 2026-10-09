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
#ifndef _WIN32 
#include <unistd.h>
#endif
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




#include "../include/graphics_engine/base_function.hpp"




float windowWidth = 640;
float windowHeight = 480;
inline void exit_failure(int code = EXIT_FAILURE);




#define ANIMATION_SPEED 1.e-2




int main(int argc, char** argv)
{
#pragma region Initialization
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
	glfwSetErrorCallback([](int error, const char* description) {
		fprintf(stderr, "GLFW Error %d: %s\n", error, description);
	});
	GLFWwindow* window;
	if(!glfwInit())
		exit_failure();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




	window = glfwCreateWindow(windowWidth, windowHeight, "Graphics", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLFWimage images[1];
	images[0].pixels = stbi_load("../../Graphics/assets/icon.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);

	glfwSwapInterval(1);




#ifdef _DEBUG
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
#endif
	glewExperimental = GL_TRUE;
	GLenum glewErr = glewInit();
	glGetError();
	if (glewErr != GLEW_OK)
	{
		std::cerr<<"GLEW init failed: "<< glewGetErrorString(glewErr) << std::endl;
		exit_failure();
	}

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
	int64_t milliseconds_since_epoch_count;
	
	Zak::VertexArrayObject vertexArrayObject;
	Zak::VertexBufferLayout layout;
	layout.Push<float>(2);

while (!glfwWindowShouldClose(window))
	{
		duration_since_epoch = now.time_since_epoch();
		milliseconds_since_epoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);
		milliseconds_since_epoch_count = milliseconds_since_epoch.count();

#pragma region Animation
#pragma endregion

#pragma region Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3,0.3,0.3,1.0);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);


		// ENABLE SHADER BEFORE DRAW ANYTHING !1!!!
		default_shader->Bind();
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
