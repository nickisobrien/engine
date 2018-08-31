#pragma once

#include "chunk.h"

class Chunk;

enum StructType
{
	Tree,
	Cactus
};

class StructureEngine
{
public:
	StructureEngine() {}
	void addStructure(Chunk *chunk, glm::ivec3 loc, StructType type);
	void generateCactus(Chunk *chunk, glm::ivec3 loc);
	void generateTree(Chunk *chunk, glm::ivec3 loc);
private:
	queue<glm::ivec3> structures;
};