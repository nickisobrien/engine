#pragma once

#include "chunk.h"
#include "FastNoise.h"

float noise(float x, float y);

class Terrain
{
public:
	Terrain(void)
	{
		this->set_noise();
	}
	void update_chunk(glm::ivec2 pos);
	void render_chunk(glm::ivec2 pos, Shader shader);
	void set_noise(void);
	void set_neighbors(glm::ivec2 pos);
	Chunk *get_chunk(glm::ivec2 pos);
private:
	unordered_map<glm::ivec2, Chunk *> world;
	FastNoise myNoise;
};