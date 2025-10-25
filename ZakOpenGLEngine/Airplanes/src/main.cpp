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
#include <thread>


using namespace Zak;

std::mutex planeMutex;
std::mutex objectAnimationMutex;
std::atomic<bool> appClosed(false);
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
	const char* XDG_CURRENT_DESKTOP = getenv("XDG_CURRENT_DESKTOP");
	if (XDG_CURRENT_DESKTOP && strcmp(XDG_CURRENT_DESKTOP, "GNOME") == 0)
		glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
	else
		glfwInitHint(GLFW_PLATFORM, GLFW_ANY_PLATFORM);
	
	window = glfwCreateWindow(windowWidth, windowHeight, "Airplane", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLFWimage images[1];
	images[0].pixels = stbi_load("../../Airplanes/assets/icon.png", &images[0].width, &images[0].height, 0, 4);
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
					while (!objectAnimationMutex.try_lock());
					while(!planeMutex.try_lock());
					auto& vertices = planeQuadrangle_ptr->GetVertices();
					vertices[0].m_pos.y += 0.1;
					vertices[1].m_pos.y += 0.1;
					vertices[2].m_pos.y += 0.1;
					vertices[3].m_pos.y += 0.1;
					planeQuadrangle_ptr->ReBind();
					planeMutex.unlock();
					objectAnimationMutex.unlock();
				}
			}
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

	std::thread objectAnimationThread([&planeQuadrangle](){
#ifdef _DEBUG
		std::cout<<std::this_thread::get_id()<<std::endl;
#endif // _DEBUG

		while (!appClosed.load())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
			while(!objectAnimationMutex.try_lock());

			while (!planeMutex.try_lock());
			auto& vertices = planeQuadrangle.GetVertices();
			vertices[0].m_pos.y -= 0.001;
			vertices[1].m_pos.y -= 0.001;
			vertices[2].m_pos.y -= 0.001;
			vertices[3].m_pos.y -= 0.001;
			planeMutex.unlock();

			objectAnimationMutex.unlock();
		}
	});

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
		while (!objectAnimationMutex.try_lock());
		planeQuadrangle.ReBind();
		objectAnimationMutex.unlock();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0,0.3,0.3,1.0);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		Renderer::GetInstance().Draw(&quadrangleTexture, Uniform("u_Color", UniformVec4(1.f, 1.f, 1.f, 1.f)));

		while(!objectAnimationMutex.try_lock());
		Renderer::GetInstance().Draw(planeQuadrangle_ptr, Uniform("u_Color", UniformVec4(1.f, 1.f, 1.f, 1.f)));
		objectAnimationMutex.unlock();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		now = std::chrono::system_clock::now();
	}

	appClosed.store(true);
	objectAnimationThread.join();

	glfwTerminate();

	return EXIT_SUCCESS;
}

inline void exit_failure(int code)
{
	glfwTerminate();
	exit(code);
}
