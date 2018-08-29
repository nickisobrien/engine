#pragma once

#include "chunk.h"
#include "FastNoise.h"
#include "lightEngine.h"

class Player;

float noise(float x, float y);

class Terrain
{
public:
	inline Terrain(void) { this->setNoise(); }
	void updateChunk(glm::ivec2 pos);
	bool renderChunk(glm::ivec2 pos, Shader shader);
	bool renderWaterChunk(glm::ivec2 pos, Shader shader);
	void setNoise(void);
	void setNeighbors(glm::ivec2 pos);
	unordered_map<glm::ivec2, Chunk *> world;
	glm::ivec2 updateList;
	LightEngine lightEngine;
private:
	FastNoise temperatureNoise;
	FastNoise humidityNoise;
	FastNoise terrainNoise;
};