#include "engine.h"
#include "terrain.h"

void Terrain::update_chunk(glm::ivec2 pos)
{
	if (this->world.find(pos) != this->world.end())
		this->world[pos]->update();
	else
	{
		this->world[pos] = new Chunk(pos.x, pos.y);
		this->world[pos]->set_terrain();
		this->world[pos]->update();
	}
}

void Terrain::render_chunk(glm::ivec2 pos, Shader shader)
{
	if (this->world.find(pos) != this->world.end())
	{
		this->world[pos]->render(shader);
	}
	else
		update_chunk(pos);// TODO add to generate list
}