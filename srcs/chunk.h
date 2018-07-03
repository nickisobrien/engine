#pragma once

#include "shader.h"

#define AIR_BLOCK 0
#define GRASS_BLOCK 1

#define CHUNK_Z 16
#define CHUNK_X 16
#define CHUNK_Y 5

int airblock_near(int chunk[CHUNK_X][CHUNK_Y][CHUNK_Z], int x, int y, int z);

class Chunk
{
public:
	void init_chunk(void);
	int chunk[CHUNK_X][CHUNK_Y][CHUNK_Z];
	int airblock_near(int chunk[CHUNK_X][CHUNK_Y][CHUNK_Z], int x, int y, int z);
	void draw_chunk(Shader shader);
	void draw_neighbors(Shader shader, int round);
	int xoff;
	int zoff;
	bool init = false;
	Chunk *mxneighbor = NULL;
	Chunk *pxneighbor = NULL;
	Chunk *mzneighbor = NULL;
	Chunk *pzneighbor = NULL;
private:
};