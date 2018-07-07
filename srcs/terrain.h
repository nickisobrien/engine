#pragma once

#include "chunk.h"

float noise(float x, float y);

class Terrain
{
public:
	void update_chunk(glm::ivec2 pos);
	void render_chunk(glm::ivec2 pos, Shader shader);
private:
	unordered_map<glm::ivec2, Chunk *> world;
};