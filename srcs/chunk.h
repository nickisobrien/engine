#pragma once

#include "shader.h"
#include "block.h"

#define CHUNK_Z 16
#define CHUNK_X 16
#define CHUNK_Y 16

enum ChunkState
{
	GENERATE,
	UPDATE,
	RENDER,
	UNLOAD
};

class Chunk
{
public:
	Chunk(int xoff = 0, int zoff = 0);
	~Chunk();
	void update(void);
	void render(Shader shader);
	bool touchingAir(int x, int y, int z);
	void buildVAO(void);
	float *getVertices(void);
	size_t getSizeVertices(void);
	void add_face(int face, int x, int y, int z);
	void cleanVAO(void);
	void setXMinus(Chunk *chunk);
	void setXPlus(Chunk *chunk);
	void setZMinus(Chunk *chunk);
	void setZPlus(Chunk *chunk);
private:
	Block ***blocks;
	glm::mat4 offsetMatrix;
	unsigned int VAO;
	unsigned int VBO_VERT;
	vector<glm::vec3> points;
	ChunkState state = GENERATE;
	Chunk *xMinus = NULL;
	Chunk *xPlus = NULL;
	Chunk *zMinus = NULL;
	Chunk *zPlus = NULL;
};