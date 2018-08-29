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
#include <glm/gtx/hash.hpp> // for unordered_map
#include <unordered_map>
#include <cmath>
#include <thread>
#include <string>
#include <map>
#include <queue>

// #define WIDTH 720
// #define HEIGHT 480

#define WIDTH 1280
#define HEIGHT 720
#define MAP(VAL,A,B,AA,BB) ((VAL - A) / (B - A)) * (BB - AA) + AA;
#define ATLAS_SIZE 256
#define TEXTURE_SIZE 16
#define DIST(X,Y,XX,YY) ((YY-Y)/(XX-X));

#define WATER_LEVEL 32

#include "blockIndex.h" // block types
#include "cubeMap.h"

using namespace std; // should move to cpp files for locality

#include "chunk.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path);
unsigned int TextureFromFile(const char *path, const string &directory);
unsigned int skybox(void);
unsigned int loadCubemap(vector<std::string> faces);
float noise(float x, float y);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void diamondSquare(int Array[CHUNK_X][CHUNK_Z], int size);