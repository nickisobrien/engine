#pragma once

#include "engine.h"
#include "camera.h"
#include "chunk.h"
#include "terrain.h"

class Player
{
public:
	inline Player(glm::vec3 pos, Terrain *terr) { camera = Camera(pos); this->terr = terr; }
	Camera camera;
	Chunk *getChunk();
	void processInput(GLFWwindow *window, float deltaTime);
	inline void setPosition(glm::vec3 pos) { this->camera.SetPosition(pos); }
	inline glm::vec3 getPosition(void) { return (this->camera.GetPosition()); }
	void update(float time)
	{
		this->applyGravity(time);
	}
	void applyGravity(float time)
	{
		glm::vec3 current = this->getPosition();
		if (!this->isGrounded())
			current.y -= this->gravity * time;
		this->setPosition(current);
	}
	bool isGrounded()
	{
		glm::vec3 current = this->getPosition();
		int x = (int)floor(current.x) % CHUNK_X;
		int y = floor(current.y);
		int z = (int)floor(current.z) % CHUNK_Z;
		if (x < 0)
			x = CHUNK_X + x;
		if (z < 0)
			z = CHUNK_Z + z;

		// cout << "Block:  (" << x << ", " << y << ", " << z << ")" << endl;
  		// cout << "Player: (" << current.x << ", " << current.y << ", " << current.z << ")" << endl;
		Block *b = getChunk()->get_block(x,y-1,z);
		if (b != NULL && (b->isActive()))
			return (true);
		return (false);
	}
	void jump(void)
	{
		glm::vec3 cur = this->getPosition();
		if (this->isGrounded())
			this->setPosition(glm::vec3(cur.x, cur.y+5.0f, cur.z));
	}
private:
	Terrain *terr;
	// float speed = 6.0f;
	// float jumpSpeed = 8.0f;
	float gravity = 9.81f;
	// glm::vec3 moveDirection = glm::vec3(0.0f);
};