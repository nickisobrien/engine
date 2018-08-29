#include "engine.h"
#include "terrain.h"

void Terrain::addedLighting()
{
	Chunk *chunk = NULL;
	// Currently not handling chunk edges
	while(lightBfsQueue.empty() == false)
	{
		// Get a reference to the front node. 
		LightNode &node = lightBfsQueue.front();
		chunk = node.chunk;
		// Pop the front node off the queue. We no longer need the node reference
		lightBfsQueue.pop();
		// Grab the light level of the current node
		short lightLevel = chunk->getTorchLight(node.x, node.y, node.z);

		// x-1
		if (node.x - 1 >= 0)
		{
			if (chunk->getTorchLight(node.x - 1, node.y, node.z) + 2 <= lightLevel)
			{
				// Set its light level
				chunk->setTorchLight(node.x - 1, node.y, node.z, lightLevel - 1);
				// Emplace new node to queue. (could use push as well)
				if (!chunk->getBlock(node.x - 1, node.y, node.z)->isActive())
					lightBfsQueue.emplace(node.x - 1, node.y, node.z, chunk);
			}
		}
		// y-1
		if (node.y - 1 >= 0)
		{
			if (chunk->getTorchLight(node.x, node.y - 1, node.z) + 2 <= lightLevel)
			{
				// Set its light level
				chunk->setTorchLight(node.x, node.y - 1, node.z, lightLevel - 1);
				// Emplace new node to queue. (could use push as well)
				if (!chunk->getBlock(node.x, node.y - 1, node.z)->isActive())
					lightBfsQueue.emplace(node.x, node.y - 1, node.z, chunk);
			}
		}
		// z-1
		if (node.z - 1 >= 0)
		{
			if (chunk->getTorchLight(node.x, node.y, node.z - 1) + 2 <= lightLevel)
			{
				// Set its light level
				chunk->setTorchLight(node.x, node.y, node.z - 1, lightLevel - 1);
				// Emplace new node to queue. (could use push as well)
				if (!chunk->getBlock(node.x, node.y, node.z - 1)->isActive())
					lightBfsQueue.emplace(node.x, node.y, node.z - 1, chunk);
			}
		}
		// x+1
		if (node.x + 1 < CHUNK_X)
		{
			if (chunk->getTorchLight(node.x + 1, node.y, node.z) + 2 <= lightLevel)
			{
				// Set its light level
				chunk->setTorchLight(node.x + 1, node.y, node.z, lightLevel - 1);
				// Emplace new node to queue. (could use push as well)
				if (!chunk->getBlock(node.x + 1, node.y, node.z)->isActive())
					lightBfsQueue.emplace(node.x + 1, node.y, node.z, chunk);
			}
		}
		// y+1
		if (node.y + 1 < CHUNK_Y)
		{
			if (chunk->getTorchLight(node.x, node.y + 1, node.z) + 2 <= lightLevel)
			{
				// Set its light level
				chunk->setTorchLight(node.x, node.y + 1, node.z, lightLevel - 1);
				// Emplace new node to queue. (could use push as well)
				if (!chunk->getBlock(node.x, node.y + 1, node.z)->isActive())
					lightBfsQueue.emplace(node.x, node.y + 1, node.z, chunk);
			}
		}
		// z+1
		if (node.z + 1 < CHUNK_Z)
		{
			if (chunk->getTorchLight(node.x, node.y, node.z + 1) + 2 <= lightLevel)
			{
				// Set its light level
				chunk->setTorchLight(node.x, node.y, node.z + 1, lightLevel - 1);
				// Emplace new node to queue. (could use push as well)
				if (!chunk->getBlock(node.x, node.y, node.z + 1)->isActive())
					lightBfsQueue.emplace(node.x, node.y, node.z + 1, chunk);
			}
		}
	}
	if (chunk)
		chunk->update();
}

void Terrain::removedLighting()
{
	while(lightRemovalBfsQueue.empty() == false)
	{
		// Get a reference to the front node
		LightRemovalNode &node = lightRemovalBfsQueue.front();
		int lightLevel = (int)node.val;
		Chunk *chunk = node.chunk;
		// Pop the front node off the queue.
		lightRemovalBfsQueue.pop();

		// x-1
		if (node.x - 1 >= 0)
		{
			int neighborLevel = node.chunk->getTorchLight(node.x - 1, node.y, node.z);
			if (neighborLevel != 0 && neighborLevel < lightLevel) {
				// Set its light level
				node.chunk->setTorchLight(node.x - 1, node.y, node.z, 0);
				// Emplace new node to queue. (could use push as well)
				lightRemovalBfsQueue.emplace(node.x - 1, node.y, node.z, neighborLevel, chunk);
			}
			else if (neighborLevel >= lightLevel)
			{
				// Add it to the update queue, so it can propagate to fill in the gaps
				// left behind by this removal. We should update the lightBfsQueue after
				// the lightRemovalBfsQueue is empty.
				lightBfsQueue.emplace(node.x - 1, node.y, node.z, chunk);
			}
		}
		// y-1
		if (node.y - 1 >= 0)
		{
			int neighborLevel = node.chunk->getTorchLight(node.x, node.y - 1, node.z);
			if (neighborLevel != 0 && neighborLevel < lightLevel) {
				// Set its light level
				node.chunk->setTorchLight(node.x, node.y - 1, node.z, 0);
				// Emplace new node to queue. (could use push as well)
				lightRemovalBfsQueue.emplace(node.x, node.y - 1, node.z, neighborLevel, chunk);
			}
			else if (neighborLevel >= lightLevel)
			{
				// Add it to the update queue, so it can propagate to fill in the gaps
				// left behind by this removal. We should update the lightBfsQueue after
				// the lightRemovalBfsQueue is empty.
				lightBfsQueue.emplace(node.x, node.y - 1, node.z, chunk);
			}
		}
		// z-1
		if (node.z - 1 >= 0)
		{
			int neighborLevel = node.chunk->getTorchLight(node.x, node.y, node.z - 1);
			if (neighborLevel != 0 && neighborLevel < lightLevel) {
				// Set its light level
				node.chunk->setTorchLight(node.x, node.y, node.z - 1, 0);
				// Emplace new node to queue. (could use push as well)
				lightRemovalBfsQueue.emplace(node.x, node.y, node.z - 1, neighborLevel, chunk);
			}
			else if (neighborLevel >= lightLevel)
			{
				// Add it to the update queue, so it can propagate to fill in the gaps
				// left behind by this removal. We should update the lightBfsQueue after
				// the lightRemovalBfsQueue is empty.
				lightBfsQueue.emplace(node.x, node.y, node.z - 1, chunk);
			}
		}

		// x+1
		if (node.x - 1 < CHUNK_X)
		{
			int neighborLevel = node.chunk->getTorchLight(node.x + 1, node.y, node.z);
			if (neighborLevel != 0 && neighborLevel < lightLevel) {
				// Set its light level
				node.chunk->setTorchLight(node.x + 1, node.y, node.z, 0);
				// Emplace new node to queue. (could use push as well)
				lightRemovalBfsQueue.emplace(node.x + 1, node.y, node.z, neighborLevel, chunk);
			}
			else if (neighborLevel >= lightLevel)
			{
				// Add it to the update queue, so it can propagate to fill in the gaps
				// left behind by this removal. We should update the lightBfsQueue after
				// the lightRemovalBfsQueue is empty.
				lightBfsQueue.emplace(node.x + 1, node.y, node.z, chunk);
			}
		}
		// y-1
		if (node.y + 1 < CHUNK_Y)
		{
			int neighborLevel = node.chunk->getTorchLight(node.x, node.y + 1, node.z);
			if (neighborLevel != 0 && neighborLevel < lightLevel) {
				// Set its light level
				node.chunk->setTorchLight(node.x, node.y + 1, node.z, 0);
				// Emplace new node to queue. (could use push as well)
				lightRemovalBfsQueue.emplace(node.x, node.y + 1, node.z, neighborLevel, chunk);
			}
			else if (neighborLevel >= lightLevel)
			{
				// Add it to the update queue, so it can propagate to fill in the gaps
				// left behind by this removal. We should update the lightBfsQueue after
				// the lightRemovalBfsQueue is empty.
				lightBfsQueue.emplace(node.x, node.y + 1, node.z, chunk);
			}
		}
		// z-1
		if (node.z + 1 < CHUNK_Z)
		{
			int neighborLevel = node.chunk->getTorchLight(node.x, node.y, node.z + 1);
			if (neighborLevel != 0 && neighborLevel < lightLevel) {
				// Set its light level
				node.chunk->setTorchLight(node.x, node.y, node.z + 1, 0);
				// Emplace new node to queue. (could use push as well)
				lightRemovalBfsQueue.emplace(node.x, node.y, node.z + 1, neighborLevel, chunk);
			}
			else if (neighborLevel >= lightLevel)
			{
				// Add it to the update queue, so it can propagate to fill in the gaps
				// left behind by this removal. We should update the lightBfsQueue after
				// the lightRemovalBfsQueue is empty.
				lightBfsQueue.emplace(node.x, node.y, node.z + 1, chunk);
			}
		}



	}

	this->addedLighting();
}

void Terrain::updateChunk(glm::ivec2 pos)
{
	if (this->world.find(pos) != this->world.end())
		this->world[pos]->update();
	else
	{
		this->world[pos] = new Chunk(pos.x, pos.y);
		this->world[pos]->setTerrain(this->terrainNoise, this->temperatureNoise, this->humidityNoise);
		this->world[pos]->update();
	}
}

bool Terrain::renderChunk(glm::ivec2 pos, Shader shader)
{
	if (this->world.find(pos) != this->world.end())
	{
		this->world[pos]->render(shader);
		if (!this->world[pos]->neighborsSet)
			this->setNeighbors(pos);
	}
	else
	{
		if (this->updateList == glm::ivec2(-100000,-100000))
			this->updateList = pos;
		return (false);
	}
	return (true);
}

bool Terrain::renderWaterChunk(glm::ivec2 pos, Shader shader)
{
	if (this->world.find(pos) != this->world.end())
		this->world[pos]->renderWater(shader);
	else
		return (false);
	return (true);
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
		this->world[pos]->addExtras(this->terrainNoise, this->temperatureNoise, this->humidityNoise);
		this->world[pos]->neighborsSet = true;
		this->world[pos]->update();
	}
}