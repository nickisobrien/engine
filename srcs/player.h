#pragma once

#include "engine.h"
#include "camera.h"

class Player
{
public:
	Player (glm::vec3 pos)
	{
		this->pos = pos;
	}
	Camera cam;
private:
	glm::vec3 pos;
}