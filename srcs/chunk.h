#pragma once

#include "shader.h"
#include "block.h"

#define CHUNK_Z 16
#define CHUNK_X 16
#define CHUNK_Y 16

class Chunk
{
public:
	Chunk(int xoff = 0, int zoff = 0);
	~Chunk();
	void update(void);
	void render(Shader shader);
	bool touchingAir(int x, int y, int z);
private:
	Block ***blocks;
	glm::mat4 offsetMatrix;
};