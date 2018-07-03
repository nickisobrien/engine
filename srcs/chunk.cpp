#include "engine.h"
#include "chunk.h"

Chunk::Chunk(int xoff, int zoff)
{
	blocks = new Block**[CHUNK_X];
	for(int i = 0; i < CHUNK_X; i++)
	{
		blocks[i] = new Block*[CHUNK_Y];

		for(int j = 0; j < CHUNK_Y; j++)
		{
			blocks[i][j] = new Block[CHUNK_Z];
		}
	}
	offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3((float)(xoff * CHUNK_X), 1.0f, (float)(zoff * CHUNK_Z)));
}

Chunk::~Chunk()
{
	// for (int i = 0; i < CHUNK_X; ++i)
	// {
	// 	for (int j = 0; j < CHUNK_Y; ++j)
	// 	{
	// 		delete [] blocks[i][j];
	// 	}

	// 	delete [] blocks[i];
	// }
	// delete [] blocks;
}

void Chunk::render(Shader shader)
{
	glm::mat4 transform = offsetMatrix;
	for(int i = 0; i < CHUNK_X; i++)
	{
		for(int j = 0; j < CHUNK_Y; j++)
		{
			for (int k = 0; k < CHUNK_Z; k++)
			{
				transform = glm::translate(transform, glm::vec3(1.0f, 0.0f, 0.0f));
				shader.setMat4("transform", transform);
				if (blocks[i][j][k].isActive() == true)
					blocks[i][j][k].render();
			}
			transform = glm::translate(transform, glm::vec3((float)-CHUNK_Z, 1.0f, 0.0f));
		}
		transform = glm::translate(transform, glm::vec3(0.0f, (float)-CHUNK_Y, 1.0f));
	}
}

void Chunk::update(void)
{
	for(int i = 0; i < CHUNK_X; i++)
	{
		for(int j = 0; j < CHUNK_Y; j++)
		{
			for (int k = 0; k < CHUNK_Z; k++)
			{
				if (touchingAir(i, j, k) == true)
					blocks[i][j][k].setActive(false);
			}
		}
	}
}

bool Chunk::touchingAir(int x, int y, int z)
{
	if (!x || !z  || !y || z == CHUNK_Z - 1 || x == CHUNK_X - 1 || y == CHUNK_Y - 1)
		return (false);
	else if (blocks[x+1][y][z].getType() == 0 || blocks[x-1][y][z].getType() == 0 ||
		blocks[x][y+1][z].getType() == 0 || blocks[x][y-1][z].getType() == 0 ||
		blocks[x][y][z+1].getType() == 0 || blocks[x][y][z-1].getType() == 0)
		return (false);
	return (true);
}