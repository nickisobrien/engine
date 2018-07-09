#pragma once

#include <GLFW/glfw3.h>
#include <math.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp> // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective 
#include <glm/gtc/type_ptr.hpp> // value_ptr
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <unordered_map>

#define WIDTH 800
#define HEIGHT 600
#define MAP(VAL,A,B,AA,BB) ((VAL - A) / (B - A)) * (BB - AA) + AA;

static float VCUBE[108] = {
	//DOWN
		1.000000, -1.000000, 1.000000,
		-1.000000, -1.000000, -1.000000,
		1.000000, -1.000000, -1.000000,
	//UP
		-1.000000, 1.000000, -1.000000,
		0.999999, 1.000000, 1.000001,
		1.000000, 1.000000, -0.999999,
	//EST
		1.000000, 1.000000, -0.999999,
		1.000000, -1.000000, 1.000000,
		1.000000, -1.000000, -1.000000,
	//SUD
		0.999999, 1.000000, 1.000001,
		-1.000000, -1.000000, 1.000000,
		1.000000, -1.000000, 1.000000,
	//OUEST
		-1.000000, -1.000000, 1.000000,
		-1.000000, 1.000000, -1.000000,
		-1.000000, -1.000000, -1.000000,
	//NORD
		1.000000, -1.000000, -1.000000,
		-1.000000, 1.000000, -1.000000,
		1.000000, 1.000000, -0.999999,

		1.000000, -1.000000, 1.000000,
		-1.000000, -1.000000, 1.000000,
		-1.000000, -1.000000, -1.000000,

		-1.000000, 1.000000, -1.000000,
		-1.000000, 1.000000, 1.000000,
		0.999999, 1.000000, 1.000001,

		1.000000, 1.000000, -0.999999,
		0.999999, 1.000000, 1.000001,
		1.000000, -1.000000, 1.000000,

		0.999999, 1.000000, 1.000001,
		-1.000000, 1.000000, 1.000000,
		-1.000000, -1.000000, 1.000000,

		-1.000000, -1.000000, 1.000000,
		-1.000000, 1.000000, 1.000000,
		-1.000000, 1.000000, -1.000000,

		1.000000, -1.000000, -1.000000,
		-1.000000, -1.000000, -1.000000,
		-1.000000, 1.000000, -1.000000
};

static float CUBEUV[72] = {
	1.000000 , 0.000000,
	0.000000 , 1.000000,
	0.000000 , 0.000000,
	1.000000 , 1.000000,
	-0.000000 , 0.000000,
	1.000000 , -0.000000,
	1.000000 , 1.000000,
	-0.000000 , 0.000000,
	1.000000 , -0.000000,
	1.000000 , 1.000000,
	-0.000000 , 0.000000,
	1.000000 , 0.000000,
	1.000000 , 0.000000,
	0.000000 , 1.000000,
	0.000000 , 0.000000,
	0.000000 , 0.000000,
	1.000000 , 1.000000,
	-0.000000 , 1.000000,
	1.000000 , 0.000000,
	1.000000 , 1.000000,
	0.000000 , 1.000000,
	1.000000 , 1.000000,
	0.000000 , 1.000000,
	-0.000000 , 0.000000,
	1.000000 , 1.000000,
	0.000000 , 1.000000,
	-0.000000 , 0.000000,
	1.000000 , 1.000000,
	0.000000 , 1.000000,
	-0.000000 , 0.000000,
	1.000000 , 0.000000,
	1.000000 , 1.000000,
	0.000000 , 1.000000,
	0.000000 , 0.000000,
	1.000000 , 0.000000,
	1.000000 , 1.000000
};

using namespace std; // should move to cpp files for locality

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path);
unsigned int TextureFromFile(const char *path, const string &directory);
unsigned int skybox(void);
unsigned int loadCubemap(vector<std::string> faces);
float noise(float x, float y);