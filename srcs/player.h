#pragma once

#include "engine.h"
#include "camera.h"
#include "chunk.h"
#include "terrain.h"

class Player
{
public:
	Player(glm::vec3 pos)
	{
		camera = Camera(pos);
	}
	Camera camera;
	Chunk *getChunk(Terrain *terr)
	{
		int cx = this->camera.Position.x >= 0.0f ? this->camera.Position.x / CHUNK_X : ceil(this->camera.Position.x) / CHUNK_X - 1.0f;
		int cz = this->camera.Position.z >= 0.0f ? this->camera.Position.z / CHUNK_Z : ceil(this->camera.Position.z) / CHUNK_X - 1.0f;
		glm::ivec2 pos(cx, cz);
		if (terr->world.find(pos) != terr->world.end())
			return (terr->world[pos]);
		return (NULL);
	}
	void processInput(GLFWwindow *window, Terrain *terr, float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glm::vec3 savePos = camera.GetPosition();

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			this->camera.ProcessKeyboard(FORWARD, deltaTime, this->getChunk(terr));
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->camera.ProcessKeyboard(BACKWARD, deltaTime, this->getChunk(terr));
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->camera.ProcessKeyboard(LEFT, deltaTime, this->getChunk(terr));
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->camera.ProcessKeyboard(RIGHT, deltaTime, this->getChunk(terr));

		glm::vec3 newPos = camera.GetPosition();

		int x = (int)floor(newPos.x) % CHUNK_X;
	    int y = floor(newPos.y);
	    int z = (int)floor(newPos.z) % CHUNK_Z;
	    if (x < 0)
	        x = CHUNK_X + x;
	    if (z < 0)
	        z = CHUNK_Z + z;

	    Block *b = getChunk(terr)->get_block(x,y,z);
	    if (b != NULL && (b->isActive())) 
	        this->camera.SetPosition(savePos);	}
private:
};