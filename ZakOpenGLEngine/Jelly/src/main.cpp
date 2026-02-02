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



#include <ZakEngine/EngineCore/Renderer.hpp>
#include <ZakEngine/EngineCore/AssetsManager.hpp>
#include <ZakEngine/Shape/Line.hpp>
#include <ZakEngine/Shape/Quadrangle.hpp>
#include <chrono>

#include "CustomShapes/Jelly.hpp"


#define ANIMATION_SPEED 1.e-2

std::mutex objectAnimationMutex;
std::atomic<bool> appClosed(false);
float windowWidth = 640;
float windowHeight = 480;
inline void exit_failure(int code = EXIT_FAILURE);
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

#pragma endregion 
#pragma region Callback functions
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
#pragma region AssetsManager
	std::shared_ptr<Zak::Texture> plane_texture;
	std::shared_ptr<Zak::Shader> texture_shader;
	std::shared_ptr<Zak::Shader> default_shader;
	try
	{
		plane_texture = Zak::AssetsManager::GetInstance().LoadTexture("plane_texture", "../../Jelly/assets/sprites/plane.png");

		texture_shader = Zak::AssetsManager::GetInstance().LoadShader("texture_shader", "../../Jelly/res/shaders/texture_shader.shader");
		default_shader = Zak::AssetsManager::GetInstance().LoadShader("default_shader", "../../Jelly/res/shaders/default_shader.shader");
	}
	catch (std::exception& e)
	{
		std::cerr<<e.what()<<std::endl;
		std::cerr<<__FILE__<<' '<< __LINE__ << ' '<< __FUNCTION__ << std::endl;
		exit_failure();
	}
#pragma endregion

	Jelly jelly;
	jelly.Init("default_shader");

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::system_clock::duration duration_since_epoch = now.time_since_epoch();
	std::chrono::milliseconds	milliseconds_since_epoch =
	std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);

	while (!glfwWindowShouldClose(window))
	{
		duration_since_epoch = now.time_since_epoch();
		milliseconds_since_epoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);

#pragma region Animation
		while (!objectAnimationMutex.try_lock());
		jelly.Animate(milliseconds_since_epoch.count(), ANIMATION_SPEED);
		objectAnimationMutex.unlock();
#pragma endregion

#pragma region Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.7,0.7,0.7,1.0);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		while(!objectAnimationMutex.try_lock());
		jelly.Draw();
		objectAnimationMutex.unlock();
#pragma endregion

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		now = std::chrono::system_clock::now();
	}

	appClosed.store(true);

	glfwTerminate();

	return EXIT_SUCCESS;
}

inline void exit_failure(int code)
{
	glfwTerminate();
	exit(code);
}
