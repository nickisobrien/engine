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

#define WIDTH 800
#define HEIGHT 600
#define MAP(VAL,A,B,AA,BB) ((VAL - A) / (B - A)) * (BB - AA) + AA;

using namespace std;