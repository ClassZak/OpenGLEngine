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
#include <thread>
#include <atomic>



#include <ZakEngine/EngineCore/Renderer.hpp>
#include <ZakEngine/EngineCore/AssetsManager.hpp>
#include <ZakEngine/Shape/Line.hpp>
#include <ZakEngine/Shape/Quadrangle.hpp>
#include <chrono>


using namespace Zak;

float windowWidth = 640;
float windowHeight = 480;
inline void exit_failure(int code = EXIT_FAILURE);
int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	GLFWwindow* window;
	if(!glfwInit())
		exit_failure();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Проверка версии OpenGL
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

	window = glfwCreateWindow(windowWidth, windowHeight, "Airplane", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		exit_failure();
	}

	GLFWimage images[1];
	images[0].pixels = stbi_load("../../Airplanes/assets/icon.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);

	glfwSwapInterval(1);


	/* Callback */
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

	
	Line<float> line({Vertex2D<float>(-1.f,-1.f),Vertex2D<float>(1.f,1.f)},GL_LINE_STRIP,5);
	line.SetShader(default_shader);

	Quadrangle quadrangle({
		Vertex2D( -0.1f, -0.1f ),
		Vertex2D( -0.1f,  0.1f ),
		Vertex2D(  0.1f,  0.1f ),
		Vertex2D(  0.1f, -0.1f ),
	});
	quadrangle.SetShader(default_shader);



	
	// Вершины треугольника
	std::vector<Vertex2D_float> vertices =
	{
		Vertex2D_float(-0.5f, -0.5f),
		Vertex2D_float( 0.5f, -0.5f),
		Vertex2D_float( 0.0f,  0.5f)
	};

	VertexBufferObject vertexBufferObject(vertices);
	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<float>(2);
	VertexArrayObject vertexArrayObject;
	vertexArrayObject.AddBuffer(vertexBufferObject, vertexBufferLayout);
	IndexBufferObject indexBufferObject({0,1,2});
	indexBufferObject.UnBind();
	vertexBufferObject.UnBind();
	vertexArrayObject.UnBind();
	

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
		glClearColor(1.0,0.3,0.3,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		// Очистка экрана

		/*Renderer::GetInstance().Draw
		(
			vertexArrayObject, 
			vertexBufferObject, 
			indexBufferObject, 
			(Shader&)(*((Shader*)default_shader.get())), 
			Uniform("u_Color", UniformVec4(0.3f,1.f,1.f,1.f))
		);*/
		vertexArrayObject.Bind();
		vertexBufferObject.Bind();
		indexBufferObject.Bind();
		default_shader->Bind();
		default_shader->SetUniform(Uniform("u_Color", UniformVec4(1.f, 1.f, 0.f, 1.f)));
		glDrawElements(GL_TRIANGLES, indexBufferObject.GetCount(), GL_UNSIGNED_INT, nullptr);

		//Renderer::GetInstance().Draw(&quadrangle, Uniform("u_Color", UniformVec4(1.f,1.f,1.f,1.f)));

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
