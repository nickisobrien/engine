#ifndef __ENGINE_H
#define __ENGINE_H

#define WIDTH 800
#define HEIGHT 600

#include <GLFW/glfw3.h>
#include <math.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp> //vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <glm/gtc/type_ptr.hpp> //value_ptr

//By defining STB_IMAGE_IMPLEMENTATION the preprocessor modifies the header file such
//that it only contains the relevant definition source code, effectively turning the header file into a .cpp file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //https://github.com/nothings/stb/blob/master/stb_image.h
#include "shader.h"
#include "camera.h"

#endif