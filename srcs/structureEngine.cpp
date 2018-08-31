#include "engine.h"
#include "structureEngine.h"

void StructureEngine::generateTree(Chunk *chunk, int x, int y, int z)
{
	// trunk
	for (int i = 0; i < 6; i++)
	{
		chunk->blocks[x][y+i][z].setType(DIRT_BLOCK);
	}
	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			chunk->blocks[x+i][y+6][z+j].setType(GRASS_BLOCK);
		}
	}
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			chunk->blocks[x+i][y+7][z+j].setType(GRASS_BLOCK);
		}
	}
}

void StructureEngine::generateCactus(Chunk *chunk,int x,int y, int z)
{
	for (int i = 0; i < 3; i++)
	{
		chunk->blocks[x][i+y][z].setType(GRASS_BLOCK);
	}
}

