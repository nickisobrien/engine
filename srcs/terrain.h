#pragma once

#include "chunk.h"
#include "FastNoise.h"

class Player;

float noise(float x, float y);

struct LightNode
{
	LightNode(int _x, int _y, int _z, Chunk* ch) : x(_x), y(_y), z(_z), chunk(ch) {}
	int x,y,z;
	// called linearizing index = y * CHUNK_HEIGHT * CHUNK_Z + z * CHUNK_X + x;
	// unsigned int index; //this is the x y z coordinate!
	Chunk *chunk; //pointer to the chunk that owns it!
};


struct LightRemovalNode
{
	LightRemovalNode(int _x, int _y, int _z, short v, Chunk* ch) : x(_x), y(_y), z(_z), val(v), chunk(ch) {}
	int x,y,z;
	short val;
	Chunk *chunk; //pointer to the chunk that owns it!
};

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
	queue<LightNode> lightBfsQueue;
	queue<LightRemovalNode> lightRemovalBfsQueue;
	void addedLighting();
	void removedLighting();
private:
	FastNoise temperatureNoise;
	FastNoise humidityNoise;
	FastNoise terrainNoise;
};