#pragma once

#include "chunk.h"

class Chunk;

class StructureEngine
{
public:
	StructureEngine() {}
	void addStructures(Chunk *chunk, int x, int y, int z);
	void generateCactus(Chunk *chunk,int x,int y, int z);
	void generateTree(Chunk *chunk,int x,int y, int z);
private:
	queue<glm::ivec3> structures;
};