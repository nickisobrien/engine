#include "engine.h"
#include "terrain.h"

void Terrain::updateChunk(glm::ivec2 pos)
{
	string p = to_string(pos.x) + "," + to_string(pos.y);
	if (this->world.find(p) != this->world.end())
		this->world[p]->update();
	else
	{
		this->world[p] = new Chunk(pos.x, pos.y);
		this->world[p]->setTerrain(this->terrainNoise, this->temperatureNoise, this->humidityNoise);
		this->world[p]->update();
	}
}

void Terrain::renderChunk(glm::ivec2 pos, Shader shader)
{
	string p = to_string(pos.x) + "," + to_string(pos.y);
	if (this->world.find(p) != this->world.end())
	{
		this->world[p]->render(shader);
		if (!this->world[p]->neighborsSet)
			this->setNeighbors(pos);
	}
	else
	{
		// check if not already in the update list
		if (find(updateList.begin(), updateList.end(), pos) == updateList.end())
			this->updateList.push_back(pos);
		// this->updateChunk(pos); // TODO: add to generate list
		// this->world[pos]->render(shader);
	}
}

void Terrain::renderWaterChunk(glm::ivec2 pos, Shader shader)
{
	string p = to_string(pos.x) + "," + to_string(pos.y);
	if (this->world.find(p) != this->world.end())
	{
		this->world[p]->renderWater(shader);
		if (!this->world[p]->neighborsSet)
			this->setNeighbors(pos);
	}
	// else
	// {
	// 	this->updateChunk(pos); // TODO: add to generate list
	// 	this->world[pos]->renderWater(shader);
	// }
}

void Terrain::setNoise(void)
{
	this->terrainNoise.SetSeed(std::time(0));
	this->terrainNoise.SetNoiseType(FastNoise::PerlinFractal);
	this->terrainNoise.SetFrequency(0.005f);

	this->temperatureNoise.SetSeed(terrainNoise.GetSeed()/2);
	this->temperatureNoise.SetNoiseType(FastNoise::PerlinFractal);
	this->temperatureNoise.SetFrequency(0.001f);

	this->temperatureNoise.SetSeed(terrainNoise.GetSeed()*2);
	this->humidityNoise.SetNoiseType(FastNoise::Perlin);
	this->humidityNoise.SetFrequency(0.001f);
}

void Terrain::setNeighbors(glm::ivec2 pos)
{
	string p = to_string(pos.x) + "," + to_string(pos.y);
	string px = to_string(pos.x-1) + "," + to_string(pos.y);
	string pxx = to_string(pos.x+1) + "," + to_string(pos.y);
	string py = to_string(pos.x) + "," + to_string(pos.y-1);
	string pyy = to_string(pos.x) + "," + to_string(pos.y+1);
	if (!this->world[p]->getXMinus() && this->world.find(px) != this->world.end())
		this->world[p]->setXMinus(this->world[px]);
	if (!this->world[p]->getXPlus() && this->world.find(pxx) != this->world.end())
		this->world[p]->setXPlus(this->world[pxx]);
	if (!this->world[p]->getZMinus() && this->world.find(py) != this->world.end())
		this->world[p]->setZMinus(this->world[py]);
	if (!this->world[p]->getZPlus() && this->world.find(pyy) != this->world.end())
		this->world[p]->setZPlus(this->world[pyy]);

	if (this->world[p]->getXPlus() && this->world[p]->getXMinus() &&
		this->world[p]->getZPlus() && this->world[p]->getZMinus())
	{
		this->world[p]->neighborsSet = true;
		this->world[p]->update();
	}
}