#pragma once

#include "chunk.h"

#define MAP(VAL,A,B,AA,BB) ((VAL - A) / (B - A)) * (BB - AA) + AA;

float noise(float x, float y);

class Terrain
{
public:
	int vertice_count;
	float scl;
	int rows, cols;
	vector<glm::vec3> vectvert;
	glm::vec3 vertices[20000]; // rows * cols * 2 // should make vector of chunks?
	Terrain(void);
	Terrain(int c, int r);
	void generate_coordinates(void);
	void generate_blocks(void);
	void draw_blocks(void);
private:
	vector<Chunk> chunks;
};