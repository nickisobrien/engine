#include "engine.h"
#include "structureEngine.h"

void StructureEngine::generateTree(Chunk *chunk, glm::ivec3 loc)
{
	// trunk
	for (int i = 0; i < 6; i++)
	{
		chunk->blocks[loc.x][loc.y+i][loc.z].setType(DIRT_BLOCK);
	}
	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			chunk->blocks[loc.x+i][loc.y+6][loc.z+j].setType(GRASS_BLOCK);
		}
	}
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			chunk->blocks[loc.x+i][loc.y+7][loc.z+j].setType(GRASS_BLOCK);
		}
	}
}

void StructureEngine::generateCactus(Chunk *chunk, glm::ivec3 loc)
{
	for (int i = 0; i < 3; i++)
		chunk->blocks[loc.x][i+loc.y][loc.z].setType(GRASS_BLOCK);
}

void StructureEngine::addStructure(Chunk *chunk, glm::ivec3 loc, StructType type)
{
	switch (type)
	{
		case StructType::Cactus:
			this->generateCactus(chunk, loc);
			break;
		case StructType::Tree:
			this->generateTree(chunk, loc);
			break;
	}
		
}
