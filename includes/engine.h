#ifndef __ENGINE_H
#define __ENGINE_H

#define WIDTH 800
#define HEIGHT 600
#define MAP(VAL,A,B,AA,BB) ((VAL - A) / (B - A)) * (BB - AA) + AA;

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

using namespace std;

#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "terrain.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path);
unsigned int TextureFromFile(const char *path, const string &directory);

#endif