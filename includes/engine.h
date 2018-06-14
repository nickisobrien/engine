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
#include <glm/glm.hpp>//With glm version 0.9.9 default matrix is 0 not identity so to make identity glm::mat4 mat = glm::mat4(1.0f)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//By defining STB_IMAGE_IMPLEMENTATION the preprocessor modifies the header file such
//that it only contains the relevant definition source code, effectively turning the header file into a .cpp file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //https://github.com/nothings/stb/blob/master/stb_image.h
#include "shader.h"

#endif