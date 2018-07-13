#pragma once

#include "engine.h"
#include "camera.h"
#include "chunk.h"
#include "terrain.h"

class Player
{
public:
	inline Player(glm::vec3 pos) { camera = Camera(pos); }
	Camera camera;
	Chunk *getChunk(Terrain *terr);
	void processInput(GLFWwindow *window, Terrain *terr, float deltaTime);
	inline void setPosition(glm::vec3 pos) { this->camera.SetPosition(pos); }
	inline glm::vec3 getPosition(glm::vec3 pos) { return (this->camera.GetPosition()); }
	void update()
	{
		this->applyGravity();
	}
	void applyGravity()
	{
	}
private:
};