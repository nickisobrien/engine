#include <engine.hpp>
#include <terrain.hpp>

#define CHUNKS_PER_LOOP 1

Terrain::Terrain(void)
{
	this->structureEngine = new StructureEngine();
	this->temperatureNoise = new FastNoise();
	this->humidityNoise = new FastNoise();
	this->terrainNoise1 = new FastNoise();
	this->terrainNoise2 = new FastNoise();
	this->terrainNoise3 = new FastNoise();
	this->setNoise();
	this->lightEngine = new LightEngine();
}

Terrain::~Terrain(void)
{
	delete this->structureEngine;
	delete this->temperatureNoise;
	delete this->humidityNoise;
	delete this->terrainNoise1;
	delete this->terrainNoise2;
	delete this->terrainNoise3;
	delete this->lightEngine;
}

void Terrain::updateChunk(glm::ivec2 pos)
{
	Chunk *c;
	if ((c = this->getChunk(pos))) // built may be the interchangable with neighborsSet
	{				
		c->clearSunLightMap();
		if (!c->neighborQueue.empty())
			c->neighborQueueUnload();
	}
	else
	{ // new chunk
		c = new Chunk(pos.x, pos.y, this);
		this->world[pos] = c;
		this->setNeighbors(pos);		
		c->setTerrain();		
	}
	this->lightEngine->sunlightInit(c);		
	c->update();
}

bool Terrain::renderChunk(glm::ivec2 pos, Shader shader)
{
	Chunk *c;
	if ((c = getChunk(pos)) && c->getState() == RENDER)
	{
		c->render(shader);
		if (!c->neighborsSet)
			this->setNeighbors(pos);
	}
	else if (this->updateList.size() < CHUNKS_PER_LOOP)
	{
		this->updateList.push(pos);
		return (false);
	}
	else if ((c = getChunk(pos)) && c->getState() == UPDATE) // render till fits on updateList
		c->render(shader);
	return (true);
}

bool Terrain::renderWaterChunk(glm::ivec2 pos, Shader shader)
{
	Chunk *c;
	if ((c = getChunk(pos)))
		c->renderWater(shader);
	else
		return (false);
	return (true);
}

// init
void Terrain::setNoise(void)
{
	this->terrainNoise1->SetSeed(std::time(0));
	this->terrainNoise1->SetNoiseType(FastNoise::PerlinFractal);
	this->terrainNoise1->SetFrequency(0.004f); // hills
	this->terrainNoise1->SetFractalOctaves(1);
	this->terrainNoise1->SetFractalGain(0.3f);

	this->terrainNoise2->SetSeed(this->terrainNoise1->GetSeed());
	this->terrainNoise2->SetNoiseType(FastNoise::PerlinFractal);
	this->terrainNoise2->SetFrequency(0.004f); // hills
	this->terrainNoise2->SetFractalOctaves(2);
	this->terrainNoise2->SetFractalGain(0.3f);

	this->terrainNoise3->SetSeed(this->terrainNoise1->GetSeed());
	this->terrainNoise3->SetNoiseType(FastNoise::PerlinFractal);
	this->terrainNoise3->SetFrequency(0.004f); // hills
	this->terrainNoise3->SetFractalOctaves(3);
	this->terrainNoise3->SetFractalGain(0.3f);

	this->temperatureNoise->SetSeed(terrainNoise1->GetSeed()/2);
	this->temperatureNoise->SetNoiseType(FastNoise::PerlinFractal);
	this->temperatureNoise->SetFrequency(0.001f);

	this->temperatureNoise->SetSeed(terrainNoise1->GetSeed()*2);
	this->humidityNoise->SetNoiseType(FastNoise::Perlin);
	this->humidityNoise->SetFrequency(0.001f);
}


void Terrain::setNeighbors(glm::ivec2 pos)
{
	Chunk *c = this->world[pos];
	Chunk *t;
	if (!c->getXMinus() && (t = getChunk(glm::ivec2(pos.x-1, pos.y))))
		c->setXMinus(t);

	if (!c->getXPlus() && (t = getChunk(glm::ivec2(pos.x+1, pos.y))))
		c->setXPlus(t);

	if (!c->getZMinus() && (t = getChunk(glm::ivec2(pos.x, pos.y-1))))
		c->setZMinus(t);

	if (!c->getZPlus() && (t = getChunk(glm::ivec2(pos.x, pos.y+1))))
		c->setZPlus(t);

	if (c->getXPlus() && c->getXMinus() && c->getZPlus() && c->getZMinus())
	{
		c->neighborsSet = true;
		if (!c->neighborQueue.empty())
			c->neighborQueueUnload();
	}
}