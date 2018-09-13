#pragma once

#include "chunk.h"


// inspired by https://www.seedofandromeda.com/blogs/29-fast-flood-fill-lighting-in-a-blocky-voxel-game-pt-1
// and         https://www.seedofandromeda.com/blogs/30-fast-flood-fill-lighting-in-a-blocky-voxel-game-pt-2
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



class LightEngine
{
public:
	void addedLighting();
	void removedLighting();
	void sunlightQueueClear();
	void sunlightInit(Chunk *c);
	queue<LightNode> lightBfsQueue;
	queue<LightRemovalNode> lightRemovalBfsQueue;
	queue <LightNode> sunlightBfsQueue;
};