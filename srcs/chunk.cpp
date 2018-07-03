#include "engine.h"
#include "chunk.h"

void Chunk::init_chunk(void)
{
	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				if (z < 60)
					chunk[x][y][z] = GRASS_BLOCK;
				else
					chunk[x][y][z] = AIR_BLOCK;
			}
		}
	}
	init = true;
}

// can switch to using this to draw only sides of the cube exposed
int Chunk::airblock_near(int chunk[CHUNK_X][CHUNK_Y][CHUNK_Z], int x, int y, int z)
{
	if (!chunk[x][y][z])
		return (0);
	else if (!x || !y || !z || x >= CHUNK_X-1 || y >= CHUNK_Y-1 || z >= CHUNK_Z-1)
		return (1);
	else if (!chunk[x+1][y][z] || !chunk[x-1][y][z] || !chunk[x][y+1][z] ||
						!chunk[x][y-1][z] || !chunk[x][y][z+1] || !chunk[x][y][z-1])
		return (1);
	return (0);
}

void Chunk::draw_chunk(Shader shader)
{
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3((float)xoff * 16.0f, 0.0f, (float)zoff * 16.0f));
	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -1.0f));
				shader.setMat4("transform", transform);
				if (airblock_near(chunk, x, y, z))
					glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			transform = glm::translate(transform, glm::vec3(0.0f, -1.0f, (float)CHUNK_Z));
		}
		transform = glm::translate(transform, glm::vec3(1.0f, (float)CHUNK_Y, 0.0f));
	}
}

void Chunk::draw_neighbors(Shader shader, int round)
{
	if (!round)
		return ;
	if (mxneighbor)
	{
		mxneighbor->draw_chunk(shader);
		mxneighbor->draw_neighbors(shader, round - 1);
	}
	if (pxneighbor)
	{
		pxneighbor->draw_chunk(shader);
		pxneighbor->draw_neighbors(shader, round - 1);
	}
	if (mzneighbor)
	{
		mzneighbor->draw_chunk(shader);
		mzneighbor->draw_neighbors(shader, round - 1);
	}
	if (pzneighbor)
	{
		pzneighbor->draw_chunk(shader);
		pzneighbor->draw_neighbors(shader, round - 1);
	}
}