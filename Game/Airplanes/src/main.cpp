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



#include <EngineCore/Renderer.hpp>
#include <EngineCore/AssetsManager.hpp>
#include <Shape/Line.hpp>
#include <Shape/Quadrangle.hpp>



std::mutex planeMutex;
QuadrangleTexture<Vertex2DText>* planeQuadrangle_ptr=nullptr;
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

	window = glfwCreateWindow(windowWidth, windowHeight, "Airplane", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLFWimage images[1];
	images[0].pixels = stbi_load("../assets/icon.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		exit_failure();

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
	glfwSetMouseButtonCallback
	(
		window,
		[](GLFWwindow* window, int button, int action, int mods)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				std::cout<<"GLFW_MOUSE_BUTTON_LEFT"<<'\n';
				if (planeQuadrangle_ptr)
				{
					while(!planeMutex.try_lock());
					auto& vertices = planeQuadrangle_ptr->GetVertices();
					vertices[0].m_pos.y += 0.01;
					vertices[1].m_pos.y += 0.01;
					vertices[2].m_pos.y += 0.01;
					vertices[3].m_pos.y += 0.01;
					planeQuadrangle_ptr->ReBind();
					planeMutex.unlock();
				}
			}
		}
	);

#pragma region AssetsManager
	std::shared_ptr<Texture> background;
	std::shared_ptr<Texture> plane_texture;
	std::shared_ptr<Shader> texture_shader;
	try
	{
		background = AssetsManager::GetInstance().LoadTexture("background", "../../Airplanes/assets/backgrounds/background.jpg");
		plane_texture = AssetsManager::GetInstance().LoadTexture("plane_texture", "../../Airplanes/assets/sprites/plane.png");

		texture_shader = AssetsManager::GetInstance().LoadShader("texture_shader", "../../Airplanes/res/shaders/texture_shader.shader");
	}
	catch (std::exception& e)
	{
		std::cerr<<e.what()<<std::endl;
		exit_failure();
	}
#pragma endregion

	QuadrangleTexture<Vertex2DText> planeQuadrangle
	({
		Vertex2DText({ -0.1, -0.1 },	{ 0,0 }),
		Vertex2DText({ -0.1,  0.1 },	{ 0,1 }),
		Vertex2DText({  0.1,  0.1 },	{ 1,1 }),
		Vertex2DText({  0.1, -0.1 },	{ 1,0 }),
	});
	planeQuadrangle_ptr = &planeQuadrangle;
	QuadrangleTexture<Vertex2DText> quadrangleTexture
	({
		Vertex2DText({ -1, -1 },	{ 0,0 }),
		Vertex2DText({ -1,  1 },	{ 0,1 }),
		Vertex2DText({  1,  1 },	{ 1,1 }),
		Vertex2DText({  1, -1 },	{ 1,0 }),
	});
	quadrangleTexture.SetTexture(background);
	quadrangleTexture.SetShader(texture_shader);
	planeQuadrangle.SetTexture(plane_texture);
	planeQuadrangle.SetShader(texture_shader);

	while (!glfwWindowShouldClose(window))
	{
		/* Animation here */
		while(!planeMutex.try_lock());
		auto& vertices = planeQuadrangle.GetVertices();
		vertices[0].m_pos.y -= 0.001;
		vertices[1].m_pos.y -= 0.001;
		vertices[2].m_pos.y -= 0.001;
		vertices[3].m_pos.y -= 0.001;
		planeQuadrangle.ReBind();
		planeMutex.unlock();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0,0.3,0.3,1.0);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		Renderer::GetInstance().Draw(&quadrangleTexture, Uniform("u_Color", UniformVec4(1.f, 1.f, 1.f, 1.f)));
		Renderer::GetInstance().Draw(planeQuadrangle_ptr, Uniform("u_Color", UniformVec4(1.f, 1.f, 1.f, 1.f)));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}

inline void exit_failure(int code)
{
	glfwTerminate();
	exit(code);
}
