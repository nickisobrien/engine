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
	for (int i = 0; i < 8; i++)
	{
		chunk->setBlock(glm::ivec3(loc.x,loc.y+i,loc.z),DIRT_BLOCK);
	}
	// bush
	for (int i = -4; i <= 4; i++)
	{
		for (int j = -4; j <= 4; j++)
		{
			chunk->setBlock(glm::ivec3(loc.x+i,loc.y+8,loc.z+j),GRASS_BLOCK);
		}
	}
	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			chunk->setBlock(glm::ivec3(loc.x+i,loc.y+9,loc.z+j),GRASS_BLOCK);
		}
	}
}

void StructureEngine::generateGiantTree(Chunk *chunk, glm::ivec3 loc)
{
	// trunk
	for (int j = -1; j <= 1; j++)
	{
		for (int k = -1; k <= 1; k++)
		{
			for (int i = -2; i < 20; i++)
			{	
				chunk->setBlock(glm::ivec3(loc.x+j,loc.y+i,loc.z+k),DIRT_BLOCK);
			}
		}	
	}
	// bush
	for (int i = -8; i <= 8; i++)
	{
		for (int j = -8; j <= 8; j++)
		{
			chunk->setBlock(glm::ivec3(loc.x+i,loc.y+20,loc.z+j),GRASS_BLOCK);
		}
	}
	for (int i = -4; i <= 4; i++)
	{
		for (int j = -4; j <= 4; j++)
		{
			chunk->setBlock(glm::ivec3(loc.x+i,loc.y+21,loc.z+j),GRASS_BLOCK);
		}
	}
}

void StructureEngine::generateCactus(Chunk *chunk, glm::ivec3 loc)
{
	for (int i = 0; i < 3; i++)
		chunk->setBlock(glm::ivec3(loc.x,i+loc.y,loc.z),GRASS_BLOCK);
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
		case StructType::GiantTree:
			this->generateGiantTree(chunk, loc);
			break;
	}	
}