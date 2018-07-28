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
	inline void update(float time) { this->applyGravity(time); }
	void applyGravity(float time);
	bool isGrounded();
	void jump();
	void leftMouseClickEvent();
private:
	Terrain *terr;
	// float speed = 6.0f;
	// float jumpSpeed = 8.0f;
	float gravity = 9.81f;
	// glm::vec3 moveDirection = glm::vec3(0.0f);
};