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
	const int treeHeight = 40;
	const int treeLayerOneWidth = 12;
	const int treeLayerTwoWidth = 8;
	const int treeTrunkWidth = 3;
	// trunk
	for (int j = -treeTrunkWidth; j <= treeTrunkWidth; j++)
	{
		for (int k = -treeTrunkWidth; k <= treeTrunkWidth; k++)
		{
			for (int i = -2; i < treeHeight; i++) // -2 for tree depth
			{	
				chunk->setBlock(glm::ivec3(loc.x+j,loc.y+i,loc.z+k),TREE_BLOCK);
			}
		}	
	}
	// bush
	for (int i = -treeLayerOneWidth; i <= treeLayerOneWidth; i++)
	{
		for (int j = -treeLayerOneWidth; j <= treeLayerOneWidth; j++)
		{
			chunk->setBlock(glm::ivec3(loc.x+i,loc.y+treeHeight,loc.z+j),TREE_LEAF_BLOCK_1);
		}
	}
	for (int i = -treeLayerTwoWidth; i <= treeLayerTwoWidth; i++)
	{
		for (int j = -treeLayerTwoWidth; j <= treeLayerTwoWidth; j++)
		{
			chunk->setBlock(glm::ivec3(loc.x+i,loc.y+treeHeight+1,loc.z+j),TREE_LEAF_BLOCK_1);
		}
	}
}

void StructureEngine::generateGiantTree(Chunk *chunk, glm::ivec3 loc)
{
	const int treeHeight = 60;
	const int treeLayerOneWidth = 24;
	const int treeLayerTwoWidth = 16;
	const int treeTrunkWidth = 5;
	// trunk
	for (int j = -treeTrunkWidth; j <= treeTrunkWidth; j++)
	{
		for (int k = -treeTrunkWidth; k <= treeTrunkWidth; k++)
		{
			for (int i = -2; i < treeHeight; i++) // -2 for tree depth
			{	
				chunk->setBlock(glm::ivec3(loc.x+j,loc.y+i,loc.z+k),TREE_BLOCK);
			}
		}	
	}
	// bush
	for (int i = -treeLayerOneWidth; i <= treeLayerOneWidth; i++)
	{
		for (int j = -treeLayerOneWidth; j <= treeLayerOneWidth; j++)
		{
			chunk->setBlock(glm::ivec3(loc.x+i,loc.y+treeHeight,loc.z+j),TREE_LEAF_BLOCK_2);
		}
	}
	for (int i = -treeLayerTwoWidth; i <= treeLayerTwoWidth; i++)
	{
		for (int j = -treeLayerTwoWidth; j <= treeLayerTwoWidth; j++)
		{
			chunk->setBlock(glm::ivec3(loc.x+i,loc.y+treeHeight+1,loc.z+j),TREE_LEAF_BLOCK_2);
		}
	}
}

void StructureEngine::generateCactus(Chunk *chunk, glm::ivec3 loc)
{
	const int cactusHeight = 14;
	const int cactusTrunkWidth = 2;
	// trunk
	for (int j = -cactusTrunkWidth; j <= cactusTrunkWidth; j++)
	{
		for (int k = -cactusTrunkWidth; k <= cactusTrunkWidth; k++)
		{
			for (int i = -2; i < cactusHeight; i++) // -2 for tree depth
			{	
				chunk->setBlock(glm::ivec3(loc.x+j,loc.y+i,loc.z+k),GRASS_BLOCK);
			}
		}	
	}
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