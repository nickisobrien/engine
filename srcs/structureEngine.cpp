#include "engine.h"
#include "structureEngine.h"

static bool isOverflowing(int x, int y, int z)
{
	if (x < 0 || x >= CHUNK_X || y < 0 || y >= CHUNK_Y || z < 0 || z >= CHUNK_Z)
		return true;
	return false;
}

void StructureEngine::generateTree(Chunk *chunk, glm::ivec3 loc)
{
	// trunk
	for (int i = 0; i < 6; i++)
	{
		if (isOverflowing(loc.x, loc.y+i, loc.z))
			chunk->neighborQueue.push_back(blockQueue(DIRT_BLOCK,glm::ivec3(loc.x, loc.y+i, loc.z)));
		else
			chunk->blocks[loc.x][loc.y+i][loc.z].setType(DIRT_BLOCK);
	}
	for (int i = -4; i <= 4; i++)
	{
		for (int j = -4; j <= 4; j++)
		{
			if (isOverflowing(loc.x+i, loc.y+6, loc.z+j))
				chunk->neighborQueue.push_back(blockQueue(GRASS_BLOCK,glm::ivec3(loc.x+i, loc.y+6, loc.z+j)));
			else
				chunk->blocks[loc.x+i][loc.y+6][loc.z+j].setType(GRASS_BLOCK);
		}
	}
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (isOverflowing(loc.x+i, loc.y+7, loc.z+j))
				chunk->neighborQueue.push_back(blockQueue(GRASS_BLOCK,glm::ivec3(loc.x+i, loc.y+7, loc.z+j)));
			else
				chunk->blocks[loc.x+i][loc.y+7][loc.z+j].setType(GRASS_BLOCK);
		}
	}
}

void StructureEngine::generateCactus(Chunk *chunk, glm::ivec3 loc)
{
	for (int i = 0; i < 3; i++)
		chunk->blocks[loc.x][i+loc.y][loc.z].setType(GRASS_BLOCK);
	// for (int i = 0; i < 3; i++)
	// 	this->structures.emplace(chunk,GRASS_BLOCK,glm::ivec3(loc.x, loc.y+i, loc.z));
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