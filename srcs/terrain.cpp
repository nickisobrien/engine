#include "engine.h"
#include "terrain.h"

void Terrain::update_chunk(glm::ivec2 pos)
{
	if (this->world.find(pos) != this->world.end())
		this->world[pos]->update();
	else
	{
		this->world[pos] = new Chunk(pos.x, pos.y);
		this->world[pos]->set_terrain(this->myNoise);
		this->world[pos]->update();
	}
}

void Terrain::render_chunk(glm::ivec2 pos, Shader shader)
{
	if (this->world.find(pos) != this->world.end())
	{
		this->world[pos]->render(shader);
		if (!this->world[pos]->neighborsSet)
			set_neighbors(pos);
	}
	else
	{
		update_chunk(pos); // TODO: add to generate list
		this->world[pos]->render(shader);
	}
}

void Terrain::set_noise(void)
{
	myNoise.SetNoiseType(FastNoise::PerlinFractal);
}

void Terrain::set_neighbors(glm::ivec2 pos)
{
	if (!this->world[pos]->getXMinus() && this->world.find(glm::ivec2(pos.x-1, pos.y)) != this->world.end())
		this->world[pos]->setXMinus(this->world[glm::ivec2(pos.x-1, pos.y)]);
	if (!this->world[pos]->getXPlus() && this->world.find(glm::ivec2(pos.x+1, pos.y)) != this->world.end())
		this->world[pos]->setXPlus(this->world[glm::ivec2(pos.x+1, pos.y)]);
	if (!this->world[pos]->getZMinus() && this->world.find(glm::ivec2(pos.x, pos.y-1)) != this->world.end())
		this->world[pos]->setZMinus(this->world[glm::ivec2(pos.x, pos.y-1)]);
	if (!this->world[pos]->getZPlus() && this->world.find(glm::ivec2(pos.x, pos.y+1)) != this->world.end())
		this->world[pos]->setZPlus(this->world[glm::ivec2(pos.x, pos.y+1)]);

	if (this->world[pos]->getXPlus() && this->world[pos]->getXMinus() &&
		this->world[pos]->getZPlus() && this->world[pos]->getZMinus())
	{
		this->world[pos]->neighborsSet = true;
		this->world[pos]->update();
	}
}