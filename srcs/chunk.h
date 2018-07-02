#pragma once

#include "shader.h"

#define AIR_BLOCK 0
#define GRASS_BLOCK 1

#define CHUNK_Z 5
#define CHUNK_X 16
#define CHUNK_Y 16

int airblock_near(int chunk[CHUNK_X][CHUNK_Y][CHUNK_Z], int x, int y, int z);

class Chunk
{
public:
	void get_chunk(void);
	int chunk[CHUNK_X][CHUNK_Y][CHUNK_Z];
	int airblock_near(int chunk[CHUNK_X][CHUNK_Y][CHUNK_Z], int x, int y, int z);
	void draw_chunk(Shader shader, glm::mat4 transform);
private:
	Chunk *mxneighbor;
	Chunk *pxneighbor;
	Chunk *myneighbor;
	Chunk *pyneighbor;
};