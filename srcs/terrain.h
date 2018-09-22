#pragma once

#include "chunk.h"
#include "FastNoise.h"
#include "lightEngine.h"
#include "structureEngine.h"

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
	glm::ivec2 updateList = glm::ivec2(-100000,-100000);
	LightEngine lightEngine;
private:
	friend class Chunk;
	FastNoise temperatureNoise;
	FastNoise humidityNoise;
	FastNoise terrainNoise1;
	FastNoise terrainNoise2;
	FastNoise terrainNoise3;
	StructureEngine structureEngine;
};