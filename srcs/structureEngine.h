#pragma once

#include "chunk.h"

class Chunk;

class StructureEngine
{
public:
	StructureEngine() {}
	static void generateCactus(Chunk *chunk,int x,int base, int z);
	static void generateTree(Chunk *chunk,int x,int base, int z);
private:

};