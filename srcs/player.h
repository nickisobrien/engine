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
	inline glm::vec3 getPosition(void) { return (this->camera.GetPosition()); }
	void update(float time, Terrain *terr)
	{
		this->applyGravity(time, terr);
	}
	void applyGravity(float time, Terrain *terr)
	{
		glm::vec3 current = this->getPosition();
		if (!this->isGrounded(terr))
			current.y -= this->gravity * time;
		this->setPosition(current);
	}
	bool isGrounded(Terrain *terr)
	{
		glm::vec3 current = this->getPosition();
		int x = (int)floor(current.x) % CHUNK_X;
		int y = floor(current.y);
		int z = (int)floor(current.z) % CHUNK_Z;
		if (x < 0)
			x = CHUNK_X + x;
		if (z < 0)
			z = CHUNK_Z + z;

		Block *b = getChunk(terr)->get_block(x,y-1,z);
		if (b != NULL && (b->isActive()))
			return (true);
		return (false);
	}
private:
	float speed = 6.0f;
	float jumpSpeed = 8.0f;
	float gravity = 9.81f;
	glm::vec3 moveDirection = glm::vec3(0.0f);
};