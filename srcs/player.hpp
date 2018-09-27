#pragma once

#include "engine.hpp"
#include "camera.hpp"
#include "chunk.hpp"
#include "terrain.hpp"

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
	void rightMouseClickEvent();
	int currentBlockPlace = Blocktype::LIGHT_BLOCK;
private:
	Terrain *terr;
	float health = 1.0f;
	float velocity = 0.0f;
	const float gravity = 26.0f;
};