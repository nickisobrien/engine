#pragma once

#include "shader.h"
#include "block.h"
#include "FastNoise.h"
#include "structureEngine.h"
#include "terrain.h"

#define CHUNK_Z 16
#define CHUNK_X 16
#define CHUNK_Y 256
#define WATER_LEVEL 38

enum ChunkState
{
	GENERATE, // needs to be generated completely
	UPDATE, // needs an update
	RENDER, // can be rendered
	UNLOAD // needs to be unloaded
};

struct blockQueue
{
	blockQueue(Blocktype t, glm::ivec3 p) : type(t), pos(p) {}
	Blocktype type;
	glm::ivec3 pos;
};

class Chunk
{
public:
	Chunk(int x = 0, int z = 0, Terrain *t = NULL);
	~Chunk(void);
	void update();
	void render(Shader shader);
	void renderWater(Shader shader);
	void faceRendering();
	void buildVAO(void);
	void addFace(int face, int x, int y, int z, int val, vector<float> *m, int *ps);
	void cleanVAO(void);

	// state management
	inline void setState(ChunkState s) { this->state = s; }
	inline ChunkState getState() { return this->state; }
	
	// neighbors
	inline void setXMinus(Chunk *chunk) { this->xMinus = chunk; }
	inline void setXPlus(Chunk *chunk) { this->xPlus = chunk; }
	inline void setZMinus(Chunk *chunk) { this->zMinus = chunk; }
	inline void setZPlus(Chunk *chunk) { this->zPlus = chunk; }
	inline Chunk *getXMinus() { return (this->xMinus); }
	inline Chunk *getXPlus() { return (this->xPlus); }
	inline Chunk *getZMinus() { return (this->zMinus); }
	inline Chunk *getZPlus() { return (this->zPlus); }
	vector<blockQueue> neighborQueue;
	void neighborQueueUnload();

	// lighting
	inline int getSunLight(int x, int y, int z) { return (sunLightMap[x][y][z]); };
	inline void setSunLight(int x, int y, int z, int val) { sunLightMap[x][y][z] = val; };
	inline int getTorchLight(int x, int y, int z) { return (torchLightMap[x][y][z]); };
	inline void setTorchLight(int x, int y, int z, int val) { torchLightMap[x][y][z] = val; };
	inline void clearSunLightMap() { memset(this->sunLightMap, 0, sizeof(this->sunLightMap)); }

	void setTerrain();
	int	getWorld(int x, int y, int z);
	bool neighborsSet = false;
	Block *getBlock(int x, int y, int z);
	void setBlock(glm::ivec3 pos, Blocktype type);
	inline int getXOff() { return xoff; }
	inline int getZOff() { return zoff; }
private:
	friend class StructureEngine; // now StructureEngine can access private parts of Chunk
	int xoff;
	int zoff;
	
	ChunkState state = GENERATE;

	Block blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
	// TODO: switch these to one map using 4 bits each, sent to buffer as char
	char torchLightMap[CHUNK_X][CHUNK_Y][CHUNK_Z];
	char sunLightMap[CHUNK_X][CHUNK_Y][CHUNK_Z];
	glm::mat4 offsetMatrix;
	unsigned int VAO;
	unsigned int VBO;

	int pointSize;
	int transparentPointSize;

	unsigned int transparentVAO;
	unsigned int transparentVBO;

	vector<float> mesh;
	vector<float> transparentMesh;

	// ChunkState state = GENERATE;
	Chunk *xMinus = NULL;
	Chunk *xPlus = NULL;
	Chunk *zMinus = NULL;
	Chunk *zPlus = NULL;

	Terrain *terr;
};