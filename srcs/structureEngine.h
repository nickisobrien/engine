#pragma once

#include "chunk.h"

class Chunk;

enum StructType
{
	Tree,
	Cactus
};

// struct blockQueue
// {
// 	Chunk *c;
// 	map<i>
// }

class StructureEngine
{
public:
	StructureEngine() {}
	void addStructure(Chunk *chunk, glm::ivec3 loc, StructType type);
private:
	void generateCactus(Chunk *chunk, glm::ivec3 loc);
	void generateTree(Chunk *chunk, glm::ivec3 loc);
	void handleChunkOverflow(Chunk *c);
};