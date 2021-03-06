#pragma once

#include "chunk.hpp"
#include "FastNoise.hpp"
#include "lightEngine.hpp"
#include "structureEngine.hpp"

class Player;

float noise(float x, float y);

class Terrain
{
public:
	Terrain(void);
	~Terrain(void);
	inline Chunk *getChunk(glm::ivec2 pos) { if (this->world.find(pos) != this->world.end()) return (this->world[pos]); return NULL; }
	void updateChunk(glm::ivec2 pos);
	bool renderChunk(glm::ivec2 pos, Shader shader);
	bool renderWaterChunk(glm::ivec2 pos, Shader shader);
	void setNoise(void);
	void setNeighbors(glm::ivec2 pos);
	unordered_map<glm::ivec2, Chunk *> world;
	stack<glm::ivec2> updateList;
	LightEngine *lightEngine;
private:
	friend class Chunk;
	FastNoise *temperatureNoise;
	FastNoise *humidityNoise;
	FastNoise *terrainNoise1;
	FastNoise *terrainNoise2;
	FastNoise *terrainNoise3;
	StructureEngine *structureEngine;
};