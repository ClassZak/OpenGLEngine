#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <tiny_obj_loader.h>

#include <string>
#include <fstream>

#include <iostream>
#include <locale>
#include <chrono>
#include <cmath>
#include <vector>
#include <array>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#endif
#include <stdlib.h>

#include <thread>
#include <mutex>
#include <functional>
#include <utility>

#include <math.h>



#include <stb_image.h>



#include "utils/GLMacro.h"

#include "Vertex/Vertex2D.hpp"
#include "Vertex/Vertex3DText.hpp"
#include "Vertex/Vertex2DText.hpp"
#include "Vertex/VertexUtils.hpp"

#include "OpenGLClass/Shader.hpp"
#include "Shape/Circle.hpp"
#include "Shape/Jelly.hpp"
#include "Shape/JellyWithTexture.hpp"
#include "Shape/Quadrangle.hpp"
#include "Shape/CircleSector.hpp"
#include "Shape/SimpleRect.hpp"

#include "OpenGLClass/VertexBufferObject.hpp"
#include "OpenGLClass/IndexBufferObject.hpp"
#include "OpenGLClass/VertexArrayObject.hpp"

#include "OpenGLClass/Shader.hpp"
#include "OpenGLClass/Texture.hpp"
#include "OpenGLClass/Mesh.hpp"

#include "EngineCore/Renderer.hpp"
#include "EngineCore/AssetsManager.hpp"



#define FPS 60
#define VERTEX_ATTRIBUTE_SIZE 2

#define ANIMATION_SPEED 1.e-3


std::vector<std::function<void(GLFWwindow* window, double xpos, double ypos)>> mouseMovingProcs;
float windowWidth	=640;
float windowHeight	=480;


/*Вид и проекция*/
const float DEFAULT_CAMERA_SPEED = .4f;
float camera_speed = DEFAULT_CAMERA_SPEED;
std::mutex camera_mutex;
glm::vec3 camera_pos = glm::vec3(-100.0f, 0.0f, 0.0f);
glm::mat4 model = glm::mat4(1.f);
glm::mat4 view = glm::lookAt
(
	camera_pos, // позиция камеры
	glm::vec3(0.0f, 0.0f, 0.0f), // цель камеры
	glm::vec3(0.0f, 0.0f, 1.0f)  // вектор "вверх"
);
glm::mat4 projection = glm::perspective
(
	glm::radians(15.f),							// угол обзора
	(float)windowWidth / (float)windowHeight,	// соотношение сторон
	0.1f,										// ближняя плоскость отсечения
	10000.0f									// дальняя плоскость отсечения
);

bool keys[1024] = { false };

enum CameraDirection
{
	None = -1,
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down
};



static void keyboard_input();
static inline void move_camera
(glm::vec3& camera_pos, glm::mat4& view, const CameraDirection direction, float speed);
static inline void speed_up();

std::unordered_map<int, std::function<void(void)>>
keyboard_events=
{
	{GLFW_KEY_W, std::function([]()->void
	{
		move_camera(camera_pos, view, CameraDirection::Forward,		camera_speed);
	})},
	{GLFW_KEY_A, std::function([]()->void
	{
		move_camera(camera_pos, view, CameraDirection::Left,		camera_speed);
	})},
	{GLFW_KEY_S, std::function([]()->void
	{
		move_camera(camera_pos, view, CameraDirection::Backward,	camera_speed);
	})},
	{GLFW_KEY_D, std::function([]()->void
	{
		move_camera(camera_pos, view, CameraDirection::Right,		camera_speed);
	})},
	{GLFW_KEY_SPACE, std::function([]()->void
	{
		move_camera(camera_pos, view, CameraDirection::Up,			camera_speed);
	})},

	{GLFW_KEY_LEFT_SHIFT, std::function([]()->void
	{
		move_camera(camera_pos, view, CameraDirection::Down,		camera_speed);
	})},
	{GLFW_KEY_RIGHT_SHIFT, std::function([]()->void
	{
		move_camera(camera_pos, view, CameraDirection::Down,		camera_speed);
	})},
};



int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

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
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)->void
		{
			for(std::size_t i=0;i!= mouseMovingProcs.size();++i)
				if(mouseMovingProcs[i])
					mouseMovingProcs[i](window, xpos, ypos);
		}
	);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scan_code, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	});
	
	
	GLFWimage images[1];
	images[0].pixels = stbi_load("../assets/icon.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
#if FPS <= 1000
	glfwSwapInterval(1);
#endif

	if (glewInit() != GLEW_OK)
		return -1;
	std::cout<<glGetString(GL_VERSION)<<std::endl;

	AssetsManager::GetInstance().LoadShader("default_shader", "../res/shaders/shader.shader");
	auto texture_shader = 
	AssetsManager::GetInstance().LoadShader("texture_shader", "../res/shaders/texture_shader.shader");
	auto model_shader = 
	AssetsManager::GetInstance().LoadShader("model_shader", "../res/shaders/model_shader.shader");
	auto model_with_texture_shader =
	AssetsManager::GetInstance().
	LoadShader("model_with_texture_shader", "../res/shaders/model_with_texture_shader.shader");

	auto jelly_texture =
	AssetsManager::GetInstance().LoadTexture("jelly_texture", "../assets/jelly_texture.png");
	auto cube_texture =
	AssetsManager::GetInstance().LoadTexture("cube_texture", "../assets/models/cube/default.png");
	auto teapot_texture =
	AssetsManager::GetInstance().LoadTexture("teapot_texture", "../assets/models/teapot/default.png");
	auto rifle_texture =
	AssetsManager::GetInstance().LoadTexture("rifle_texture", "../assets/rifle.png");


	auto cube_model =
	AssetsManager::GetInstance().LoadMesh("cube_model", "../assets/models/cube/cube.obj", cube_texture);
	auto teapot_model =
	AssetsManager::GetInstance().LoadMesh("teapot_model", "../assets/models/teapot/teapot.obj", teapot_texture);
	teapot_model.get()->SetShader(model_with_texture_shader);
	cube_model.get()->SetShader(model_with_texture_shader);
	cube_model.get()->Move(0, -50, 0);

	JellyWithTexture jelly(jelly_texture, texture_shader);


	QuadrangleTexture<Vertex2DText> quadrangle_template(
	{ 
		Vertex2DText({0.5,	-1},	{0,0}), 
		Vertex2DText({0.5,	-0.5},	{0,1}),
		Vertex2DText({1.0,	-0.5},	{1,1}),
		Vertex2DText({1.0,	-1},	{1,0}),
	});
	quadrangle_template.SetShader(texture_shader);
	quadrangle_template.SetTexture(rifle_texture);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		std::chrono::system_clock::duration duration_since_epoch = now.time_since_epoch();
		std::chrono::milliseconds	milliseconds_since_epoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);
		std::chrono::seconds		seconds_since_epoch =
		std::chrono::duration_cast<std::chrono::seconds>(duration_since_epoch);

		long diff = 2L;
#if FPS <= 1000
		while (int(1000. / diff) >= FPS)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
			diff = std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now() - now).count();
		}
#endif  // FPS <= 1000


		keyboard_input();
		/* Animate here */
		jelly.Animate(milliseconds_since_epoch.count(), ANIMATION_SPEED);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(.7f,.7f,.7f,1.f);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		
		Renderer::GetInstance().Draw(teapot_model.get(), 
		{Uniform("model", model),Uniform("view", view), Uniform("projection", projection) });
		Renderer::GetInstance().Draw(cube_model.get(), 
		{Uniform("model", model),Uniform("view", view), Uniform("projection", projection) });
		Renderer::GetInstance().Draw(&quadrangle_template, Uniform("u_Color", UniformVec4(1.f, 1.f, 1.f, 1.f)));
		
		//Renderer::GetInstance().Draw(&jelly, { "u_Color", UniformVec4(1.f, 1.f, 1.f, 1.f) });

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();


		now = std::chrono::system_clock::now();
	}

	glfwTerminate();

	return 0;
}

void keyboard_input()
{
	for(auto& event : keyboard_events)
		if(keys[event.first])
			event.second();
}

void move_camera(glm::vec3& camera_pos, glm::mat4& view, const CameraDirection direction, float speed = 0.1f)
{
	using namespace std::chrono;

	while(!camera_mutex.try_lock())
	std::this_thread::sleep_for(milliseconds(2));

	switch (direction)
	{
		case CameraDirection::Forward:
			camera_pos.x += speed;
			break;
		case CameraDirection::Backward:
			camera_pos.x -= speed;
			break;
		case CameraDirection::Left:
			camera_pos.y += speed;
			break;
		case CameraDirection::Right:
			camera_pos.y -= speed;
			break;
		case CameraDirection::Up:
			camera_pos.z += speed;
			break;
		case CameraDirection::Down:
			camera_pos.z -= speed;
			break;
		default:
			camera_mutex.unlock();
			return;
	}


	glm::vec3 camera_target = glm::vec3(camera_pos.x + 1, camera_pos.y, camera_pos.z);
	static glm::vec3 up_vector = glm::vec3(0.f, 0.f, 1.f);
	view = glm::lookAt
	(
		camera_pos,
		camera_target,
		up_vector
	);
	printf("(%2.2f,\t%2.2f,\t%2.2f)\n", camera_pos.x, camera_pos.y, camera_pos.z);
	printf("(%2.2f,\t%2.2f,\t%2.2f)\n", camera_target.x, camera_target.y, camera_target.z);

	camera_mutex.unlock();
}

inline void speed_up()
{
	camera_speed = DEFAULT_CAMERA_SPEED * 2.5f;
}
