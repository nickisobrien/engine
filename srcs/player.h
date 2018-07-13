#pragma once

#include "engine.h"
#include "camera.h"
#include "chunk.h"
#include "terrain.h"

class Player
{
public:
	Player (glm::vec3 pos)
	{
		camera = Camera(pos);
	}
	Camera camera;
	Chunk *getChunk(Terrain *terr)
	{
		int cx = this->camera.Position.x >= 0.0f ? this->camera.Position.x / CHUNK_X : ceil(this->camera.Position.x) / CHUNK_X - 1.0f;
		int cz = this->camera.Position.z >= 0.0f ? this->camera.Position.z / CHUNK_Z : ceil(this->camera.Position.z) / CHUNK_X - 1.0f;
		glm::ivec2 pos(cx, cz);
		if (terr->world.find(pos) != terr->world.end())
			return (terr->world[pos]);
		return (NULL);
	}
private:
};