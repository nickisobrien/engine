#pragma once

#include "chunk.h"
#include "FastNoise.h"

float noise(float x, float y);

class Terrain
{
public:
	Terrain(void)
	{
		this->setNoise();
	}
	void updateChunk(glm::ivec2 pos);
	void renderChunk(glm::ivec2 pos, Shader shader);
	void setNoise(void);
	void setNeighbors(glm::ivec2 pos);
	unordered_map<glm::ivec2, Chunk *> world;
private:
	FastNoise biomeNoise;
	FastNoise terrainNoise;
};