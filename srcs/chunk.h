#pragma once

#include "shader.h"
#include "block.h"
#include "FastNoise.h"

#define CHUNK_Z 16
#define CHUNK_X 16
#define CHUNK_Y 256

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
	~Chunk(void);
	void update(void);
	void render(Shader shader);
	void renderWater(Shader shader);
	void faceRendering(void);
	void buildVAO(void);
	void addFace(int face, int x, int y, int z, int val);
	void addTransparentFace(int face, int x, int y, int z, int val);
	void cleanVAO(void);
	
	// neighbors
	void setXMinus(Chunk *chunk);
	void setXPlus(Chunk *chunk);
	void setZMinus(Chunk *chunk);
	void setZPlus(Chunk *chunk);
	Chunk *getXMinus();
	Chunk *getXPlus();
	Chunk *getZMinus();
	Chunk *getZPlus();

	// lighting
	inline int getSunlight(int x, int y, int z);
	inline void setSunlight(int x, int y, int z, int val);
	inline int getTorchlight(int x, int y, int z);
	inline void setTorchlight(int x, int y, int z, int val);

	void setTerrain(FastNoise terrainNoise, FastNoise temperatureNoise, FastNoise humidityNoise);
	void addExtras(FastNoise terrainNoise, FastNoise temperatureNoise, FastNoise humidityNoise);
	int	getWorld(int x, int y, int z);
	bool neighborsSet = false;
	Block *getBlock(int x, int y, int z);
	inline int getXOff() { return xoff; }
	inline int getZOff() { return zoff; }
private:
	friend class Player;
	int xoff;
	int zoff;
	Block blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
	char lightMap[CHUNK_X][CHUNK_Y][CHUNK_Z];
	glm::mat4 offsetMatrix;
	unsigned int VAO;
	unsigned int VBO;

	int pointSize;
	int transparentPointSize;

	unsigned int transparentVAO;
	unsigned int transparentVBO;

	vector<float> mesh;
	vector<float> transparentMesh;

	ChunkState state = GENERATE;
	Chunk *xMinus = NULL;
	Chunk *xPlus = NULL;
	Chunk *zMinus = NULL;
	Chunk *zPlus = NULL;
};