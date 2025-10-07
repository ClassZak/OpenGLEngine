#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <tiny_obj_loader.h>
#include <stb_image.h>

static float windowWidth;
static float windowHeight;
int main1();
int main2();
int main3();
inline void exit_failure(int code = 1)
{
	glfwTerminate();
	exit(code);
}

