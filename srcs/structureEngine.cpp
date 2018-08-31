#include "engine.h"
#include "structureEngine.h"

void StructureEngine::generateTree(Chunk *chunk, int x, int base, int z)
{
	// trunk
	for (int y = 0; y < 6; y++)
	{
		chunk->blocks[x][base+y][z].setType(DIRT_BLOCK);
	}
	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			chunk->blocks[x+i][base+6][z+j].setType(GRASS_BLOCK);
		}
	}
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			chunk->blocks[x+i][base+7][z+j].setType(GRASS_BLOCK);
		}
	}
}

void StructureEngine::generateCactus(Chunk *chunk,int x,int base, int z)
{
	for (int y = 0; y < 3; y++)
	{
		chunk->blocks[x][base+y][z].setType(GRASS_BLOCK);
	}
}