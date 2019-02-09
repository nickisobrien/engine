#include <engine.hpp>
#include <structureEngine.hpp>

static bool isOverflowing(int x, int y, int z)
{
	if (x < 0 || x >= CHUNK_X || y < 0 || y >= CHUNK_Y || z < 0 || z >= CHUNK_Z)
		return true;
	return false;
}

void StructureEngine::generateTree(Chunk *chunk, glm::ivec3 loc)
{
	const int treeHeight = 6;
	const int treeLayerWidth = 4;
	const int treeLayers = 2;
	const int treeTrunkWidth = 0;
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
	for (int k = 0; k < treeLayers; k++)
	{
		for (int i = -treeLayerWidth+k*2; i <= treeLayerWidth-k*2; i++)
		{
			for (int j = -treeLayerWidth+k*2; j <= treeLayerWidth-k*2; j++)
			{
				chunk->setBlock(glm::ivec3(loc.x+i,loc.y+treeHeight+k,loc.z+j),TREE_LEAF_BLOCK_1);
			}
		}
	}
}

void StructureEngine::generateGiantTree(Chunk *chunk, glm::ivec3 loc)
{
	const int treeHeight = 14;
	const int treeLayerWidth = 8;
	const int treeLayers = 2;
	const int treeTrunkWidth = 1;
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
	for (int k = 0; k < treeLayers; k++)
	{
		for (int i = -treeLayerWidth + k*4; i <= treeLayerWidth - k*4; i++)
		{
			for (int j = -treeLayerWidth + k*4; j <= treeLayerWidth - k*4; j++)
			{
				chunk->setBlock(glm::ivec3(loc.x+i,loc.y+treeHeight+k,loc.z+j),TREE_LEAF_BLOCK_2);
			}
		}
	}
}

void StructureEngine::generateCactus(Chunk *chunk, glm::ivec3 loc)
{
	const int cactusHeight = 4;
	const int cactusTrunkWidth = 0;
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

void StructureEngine::generateRock(Chunk *chunk, glm::ivec3 loc)
{
	chunk->setBlock(glm::ivec3(loc.x,loc.y,loc.z),STONE_BLOCK);
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
		case StructType::Rock:
			this->generateRock(chunk, loc);
			break;
	}	
}