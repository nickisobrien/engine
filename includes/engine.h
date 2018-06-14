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

//By defining STB_IMAGE_IMPLEMENTATION the preprocessor modifies the header file such
//that it only contains the relevant definition source code, effectively turning the header file into a .cpp file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //https://github.com/nothings/stb/blob/master/stb_image.h
#include "shader.h"

#endif