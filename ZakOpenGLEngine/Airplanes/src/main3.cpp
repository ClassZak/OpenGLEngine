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
#include <thread>


#include <ZakEngine/EngineCore/Renderer.hpp>
#include <ZakEngine/EngineCore/AssetsManager.hpp>
#include <ZakEngine/Shape/Line.hpp>
#include <ZakEngine/Shape/Quadrangle.hpp>
#include <chrono>

#include "main.hpp"

using namespace Zak;

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)
int main3()
{
	windowWidth = 640;
    windowHeight = 480;

	// Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Проверка версии OpenGL
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Airplane", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	GLFWimage images[1];
	images[0].pixels = stbi_load("../../Airplanes/assets/icon.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);

	glfwSwapInterval(1);

	/* Предустановленные Callback функции*/
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

#pragma region AssetsManager
	std::shared_ptr<Texture> background;
	std::shared_ptr<Texture> plane_texture;
	std::shared_ptr<Shader> texture_shader;
	std::shared_ptr<Shader> default_shader;
	try
	{
		background = Zak::AssetsManager::GetInstance().LoadTexture("background", "../../Airplanes/assets/backgrounds/background.png");
		plane_texture = Zak::AssetsManager::GetInstance().LoadTexture("plane_texture", "../../Airplanes/assets/sprites/plane.png");

		texture_shader = Zak::AssetsManager::GetInstance().LoadShader("texture_shader", "../../Airplanes/res/shaders/texture_shader.shader");
		default_shader = Zak::AssetsManager::GetInstance().LoadShader("default_shader", "../../Airplanes/res/shaders/default_shader.shader");
	}
	catch (std::exception& e)
	{
		std::cerr<<e.what()<<std::endl;
		exit_failure();
	}
#pragma endregion
#pragma region Rendering objects
	// Вершины треугольника
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // левая нижняя
         0.5f, -0.5f, 0.0f, // правая нижняя
         0.0f,  0.5f, 0.0f  // верхняя
    };
	 // Создание VAO, VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
#pragma endregion

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::system_clock::duration duration_since_epoch = now.time_since_epoch();
	std::chrono::milliseconds	milliseconds_since_epoch =
	std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);

	while (!glfwWindowShouldClose(window))
	{
		duration_since_epoch = now.time_since_epoch();
		milliseconds_since_epoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);

		/* Animation here */

		/* Render here */
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Темно-серый
        glClear(GL_COLOR_BUFFER_BIT);
		//glClearColor(0.0, 1.0, 0.0, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_BLEND);
		glUseProgram(default_shader.get()->GetProgram());
		default_shader->SetUniform(Uniform("u_Color", UniformVec4{1.,0.3,1.,1.}));
		glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

		//vec4(0.2, 0.6, 1.0, 1.0); // Синий цвет
		//Renderer::GetInstance().Draw(planeQuadrangle_ptr, Uniform("u_Color", UniformVec4(1.f, 1.f, 1.f, 1.f)));
		glCheckError();
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
			std::cout << "OpenGL error: " << err << std::endl;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();

		now = std::chrono::system_clock::now();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}
