#include <engine.hpp>
#include <chunk.hpp>

#define YSQRT sqrt(CHUNK_Y-1)

Chunk::Chunk(int x, int z, Terrain *t) : xoff(x), zoff(z), terr(t)
{
	this->blocks = new Block**[CHUNK_X];
	this->torchLightMap = new uint8_t**[CHUNK_X];
	this->sunLightMap = new uint8_t**[CHUNK_X];
	for(int i = 0; i < CHUNK_X; i++)
	{
		this->blocks[i] = new Block*[CHUNK_Y];
		this->torchLightMap[i] = new uint8_t*[CHUNK_Y];
		this->sunLightMap[i] = new uint8_t*[CHUNK_Y];
		for(int j = 0; j < CHUNK_Y; j++)
		{
			this->blocks[i][j] = new Block[CHUNK_Z];
			this->torchLightMap[i][j] = new uint8_t[CHUNK_Z]();
			this->sunLightMap[i][j] = new uint8_t[CHUNK_Z]();
		}
	}

	this->pointSize = 0;
	this->transparentPointSize = 0;
	offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3((float)(xoff * CHUNK_X), 1.0f, (float)(zoff * CHUNK_Z)));
	offsetMatrix = glm::translate(offsetMatrix, glm::vec3(0.5f, -0.5f, 0.5f));

	// non transparent
	// cout << x << " " << z << endl;
	// this segfaults on multithreaded generation?
	glGenVertexArrays(1, &this->VAO);
	// cout << "NO SEG!" << endl;
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		// vertices
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		
		// textures
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// normals
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		// torch light
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);

		// sun light
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
		glEnableVertexAttribArray(4);
	glBindVertexArray(0);	

	// transparent
	glGenVertexArrays(1, &this->transparentVAO);
	glGenBuffers(1, &this->transparentVBO);
	glBindVertexArray(this->transparentVAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->transparentVBO);

		// vertices
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		
		// textures
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// normals
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		// torch light
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);

		// sun light
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
		glEnableVertexAttribArray(4);
	glBindVertexArray(0);
}

int	Chunk::getWorld(int x, int y, int z)
{
	if (y < 0)
		return (0);
	if (y >= CHUNK_Y)
		return (0);
	if (x > CHUNK_Z - 1 || x < 0 || z > CHUNK_Z - 1 || z < 0)
		return (0);
	return (x + (y * CHUNK_Y) + (z * (CHUNK_Z * CHUNK_Y)));
}

Block *Chunk::getBlock(int x, int y, int z)
{
	if (x >= 0 && x < CHUNK_X && y >= 0 && y < CHUNK_Y && z >= 0 && z < CHUNK_Z)
		return (&blocks[x][y][z]);
	return (NULL);
}

void Chunk::setBlock(glm::ivec3 pos, Blocktype type)
{
	if (pos.y < 0 || pos.y >= CHUNK_Y)
		return ;
	if (pos.x < 0 ||  pos.z < 0 || pos.x >= CHUNK_X || pos.z >= CHUNK_Z)
		this->neighborQueue.push_back(blockQueue(type, pos));
	else
		this->blocks[pos.x][pos.y][pos.z].setType(type);
}

Chunk::~Chunk(void)
{
	this->cleanVAO();
	for(int i = 0; i < CHUNK_X; i++)
	{
		for(int j = 0; j < CHUNK_Y; j++)
		{
			delete this->blocks[i][j];
			delete this->torchLightMap[i][j];
			delete this->sunLightMap[i][j];
		}
		delete this->blocks[i];
		delete this->torchLightMap[i];
		delete this->sunLightMap[i];
	}
	delete this->blocks;
	delete this->torchLightMap;
	delete this->sunLightMap;
}

void Chunk::render(Shader shader)
{
	shader.setMat4("transform", this->offsetMatrix);
	shader.setFloat("transparency", 1.0f);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->pointSize);
	glBindVertexArray(0);
}

void Chunk::renderWater(Shader shader)
{
	shader.setMat4("transform", offsetMatrix);
	shader.setFloat("transparency", 0.65f);
	glBindVertexArray(transparentVAO);
	glDrawArrays(GL_TRIANGLES, 0, transparentPointSize);
	glBindVertexArray(0);
}

// could also check neighbors to see if they have any blocks for this chunk, could be faster?
void Chunk::neighborQueueUnload()
{
	vector<blockQueue> temp;
	for (int i = 0; i < neighborQueue.size(); i++)
	{
		if (neighborQueue[i].pos.x < 0 && this->getXMinus())
		{
			this->getXMinus()->setBlock(glm::ivec3(CHUNK_X+neighborQueue[i].pos.x,
				neighborQueue[i].pos.y, neighborQueue[i].pos.z), neighborQueue[i].type);
			this->getXMinus()->setState(UPDATE);
		}
		else if (neighborQueue[i].pos.x < 0)
		{
			temp.push_back(neighborQueue[i]);
		}
		else if (neighborQueue[i].pos.z < 0 && this->getZMinus())
		{
			this->getZMinus()->setBlock(glm::ivec3(neighborQueue[i].pos.x,
				neighborQueue[i].pos.y, CHUNK_Z+neighborQueue[i].pos.z), neighborQueue[i].type);
			this->getZMinus()->setState(UPDATE);
		}
		else if (neighborQueue[i].pos.z < 0)
		{
			temp.push_back(neighborQueue[i]);
		}
		else if (neighborQueue[i].pos.x >= CHUNK_X && this->getXPlus())
		{
			this->getXPlus()->setBlock(glm::ivec3(neighborQueue[i].pos.x-CHUNK_X,
				neighborQueue[i].pos.y, neighborQueue[i].pos.z), neighborQueue[i].type);
			this->getXPlus()->setState(UPDATE);
		}
		else if (neighborQueue[i].pos.x >= CHUNK_X)
		{
			temp.push_back(neighborQueue[i]);
		}

		else if (neighborQueue[i].pos.z >= CHUNK_Z && this->getZPlus())
		{
			this->getZPlus()->setBlock(glm::ivec3(neighborQueue[i].pos.x,
				neighborQueue[i].pos.y, neighborQueue[i].pos.z-CHUNK_Z), neighborQueue[i].type);
			this->getZPlus()->setState(UPDATE);
		}
		else if (neighborQueue[i].pos.z >= CHUNK_Z)
		{
			temp.push_back(neighborQueue[i]);
		}
	}
	neighborQueue.clear();
	neighborQueue = temp;
}

void Chunk::pullTerrainFromNeighbors()
{
	if (this->getXMinus())
		this->getXMinus()->neighborQueueUnload();
	if (this->getZMinus())
		this->getZMinus()->neighborQueueUnload();
	if (this->getXPlus())
		this->getXPlus()->neighborQueueUnload();
	if (this->getZPlus())
		this->getZPlus()->neighborQueueUnload();
	this->setState(RENDER); //reset state because it doesn't need the update that neighborqueueunload does
}

// heightmap generation
int	Chunk::getBase(int x, int z)
{
	float b1 = MAP(this->terr->terrainNoise1->GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff)), -1.0f, 1.0f, 0.1f, YSQRT);
	// float b2 = MAP(this->terr->terrainNoise2.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff)), -1.0f, 1.0f, 0.1f, YSQRT);
	// float b3 = MAP(this->terr->terrainNoise3.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff)), -1.0f, 1.0f, 0.1f, YSQRT);
	// return (pow((b1+b2+b3)/3, 2));
	return (pow(b1, 2));
}

void Chunk::setTerrain()
{
	// std::clock_t	start;
	// start = std::clock();

	/* PERLIN NOISE */
	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int z = 0; z < CHUNK_Z; z++)
		{
			bool water = false;
			// Use the noise library to get the height value of x, z
			int base = getBase(x,z);
			float temp = this->terr->temperatureNoise->GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff));
			float hum = this->terr->humidityNoise->GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff));
			short blocktype;
			// noise layer #1 "Temperature"
			// noise layer #2 "Humidity"
			(temp < -0.33f) ?
				(hum < 0.0f) ? blocktype = 67 : blocktype = 68
				: (temp >= 0.33f) ?
					(hum < 0.0f) ? blocktype = Blocktype::GRASS_BLOCK : blocktype = Blocktype::DIRT_BLOCK
					: (hum < 0.0f) ? blocktype = Blocktype::SAND_BLOCK : blocktype = Blocktype::GRASS_BLOCK;

			if (base < WATER_LEVEL)
				water = true;
			for (int y = 0; y < base - 4; y++)
			{
				this->blocks[x][y][z].setType(Blocktype::STONE_BLOCK);
			}
			for (int y = base - 4; y < base; y++)
			{
				this->blocks[x][y][z].setType(blocktype); // switched to grassland for now
			}
			for (int y = base; y < WATER_LEVEL; y++)
			{
				water = true;
				this->blocks[x][y][z].setType(Blocktype::WATER_BLOCK);
			}

			// extras
			if (!water)
			{
				if (blocktype == Blocktype::GRASS_BLOCK && rand() % 10000 > 9996)
					this->terr->structureEngine->addStructure(this,glm::ivec3(x,base,z), StructType::Tree);
				else if (blocktype == Blocktype::GRASS_BLOCK && rand() % 10000 > 9998)
					this->terr->structureEngine->addStructure(this,glm::ivec3(x,base,z), StructType::GiantTree);

				if (blocktype == Blocktype::SAND_BLOCK && rand() % 1000 > 998)
					this->terr->structureEngine->addStructure(this,glm::ivec3(x,base,z), StructType::Cactus);
				else if (blocktype == Blocktype::SAND_BLOCK && rand() % 1000 > 998)
					this->terr->structureEngine->addStructure(this,glm::ivec3(x,base,z), StructType::Rock);
			}
		}
	}
	// might not actually need to pull terrain from neighbors here:
	// this->pullTerrainFromNeighbors();
	// std::cout << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << std::endl;
}

void Chunk::update()
{
	this->transparentPointSize = 0;
	this->pointSize = 0;

	this->faceRendering();
	this->buildVAO();
}

void Chunk::faceRendering()
{
	bool transparent;
	int xMinusCheck;
	int yMinusCheck;
	int zMinusCheck;
	int xPlusCheck;
	int yPlusCheck;
	int zPlusCheck;
	for(int x = 0; x < CHUNK_X; x++)
	{
		for(int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				transparent = false;
				if (this->blocks[x][y][z].getType() == Blocktype::AIR_BLOCK)
					continue ;
				if (this->blocks[x][y][z].getType() == Blocktype::WATER_BLOCK) //Blocktype::water_BLOCK
					transparent = true;
				int val = this->getWorld(x, y, z);

				// MINUS checks
				if (!x && this->getXMinus())
					xMinusCheck = this->xMinus->blocks[CHUNK_X-1][y][z].getType();
				else if (!x)
				{
					int base = getBase(x-1,z);
					if (base <= y)
						xMinusCheck = 0;
					else
						xMinusCheck = 1;
				}
				else
					xMinusCheck = this->blocks[x-1][y][z].getType();

				if (!z && this->getZMinus())
					zMinusCheck = this->zMinus->blocks[x][y][CHUNK_Z-1].getType();
				else if (!z)
				{
					int base = getBase(x,z-1);
					if (base <= y)
						zMinusCheck = 0;
					else
						zMinusCheck = 1;
				}
				else
					zMinusCheck = this->blocks[x][y][z-1].getType();

				if (!y)
					yMinusCheck = 1;
				 else
				 	yMinusCheck = blocks[x][y-1][z].getType();

				 // PLUS CHECKS
				if (x == CHUNK_X-1 && this->getXPlus())
					xPlusCheck = this->xPlus->blocks[0][y][z].getType();
				else if (x == CHUNK_X-1)
				{
					int base = getBase(x+1,z);
					if (base <= y)
						xPlusCheck = 0;
					else
						xPlusCheck = 1;
				}
				else
					xPlusCheck = this->blocks[x+1][y][z].getType();

				if (z == CHUNK_Z-1 && this->getZPlus())
					zPlusCheck = this->zPlus->blocks[x][y][0].getType();
				else if (z == CHUNK_Z-1)
				{
					int base = getBase(x,z+1);
					if (base <= y)
						zPlusCheck = 0;
					else
						zPlusCheck = 1;
				}
				else
					zPlusCheck = this->blocks[x][y][z+1].getType();

				if (y == CHUNK_Y-1)
					yPlusCheck = 1;
				else
					yPlusCheck = this->blocks[x][y+1][z].getType();

				// Facing
				if (!transparent)
				{
					if (yMinusCheck==Blocktype::AIR_BLOCK || yMinusCheck==Blocktype::WATER_BLOCK)
						this->addFace(0, x , y, z, val, &this->mesh, &this->pointSize); //DOWN
					if (yPlusCheck==Blocktype::AIR_BLOCK || yPlusCheck==Blocktype::WATER_BLOCK)
						this->addFace(1, x , y, z, val, &this->mesh, &this->pointSize); //UP
					if (xPlusCheck==Blocktype::AIR_BLOCK || xPlusCheck==Blocktype::WATER_BLOCK)
						this->addFace(2, x , y, z, val, &this->mesh, &this->pointSize); //xpos SIDE
					if (zPlusCheck==Blocktype::AIR_BLOCK || zPlusCheck==Blocktype::WATER_BLOCK)
						this->addFace(3, x , y, z, val, &this->mesh, &this->pointSize); //zpos SIDE
					if (xMinusCheck==Blocktype::AIR_BLOCK || xMinusCheck==Blocktype::WATER_BLOCK)
						this->addFace(4, x , y, z, val, &this->mesh, &this->pointSize); //xneg SIDE
					if (zMinusCheck==Blocktype::AIR_BLOCK || zMinusCheck==Blocktype::WATER_BLOCK)
						this->addFace(5, x , y, z, val, &this->mesh, &this->pointSize); //zneg SIDE
				}
				else
				{
					if (yMinusCheck==Blocktype::AIR_BLOCK || (yMinusCheck==Blocktype::WATER_BLOCK && this->blocks[x][y][z].getType() != Blocktype::WATER_BLOCK))
						this->addFace(0, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //DOWN
					if (yPlusCheck==Blocktype::AIR_BLOCK || (yPlusCheck==Blocktype::WATER_BLOCK && this->blocks[x][y][z].getType() != Blocktype::WATER_BLOCK))
						this->addFace(1, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //UP
					//FOR WATER BLOCKS SIDES // if (xPlusCheck==Blocktype::AIR_BLOCK || (xPlusCheck==Blocktype::WATER_BLOCK && this->blocks[x][y][z].getType() != Blocktype::WATER_BLOCK))
					// 	this->addFace(2, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //xpos SIDE
					// if (zPlusCheck==Blocktype::AIR_BLOCK || (zPlusCheck==Blocktype::WATER_BLOCK && this->blocks[x][y][z].getType() != Blocktype::WATER_BLOCK))
					// 	this->addFace(3, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //zpos SIDE
					// if (xMinusCheck==Blocktype::AIR_BLOCK || (xMinusCheck==Blocktype::WATER_BLOCK && this->blocks[x][y][z].getType() != Blocktype::WATER_BLOCK))
					// 	this->addFace(4, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //xneg SIDE
					// if (zMinusCheck==Blocktype::AIR_BLOCK || (zMinusCheck==Blocktype::WATER_BLOCK && this->blocks[x][y][z].getType() != Blocktype::WATER_BLOCK))
					// 	this->addFace(5, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //zneg SIDE
				}
			}
		}
	}
}

void Chunk::buildVAO(void)
{
	glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), &this->mesh[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
	mesh.clear(); // don't need after mesh is built

	glBindVertexArray(this->transparentVAO);
		glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
		glBufferData(GL_ARRAY_BUFFER, transparentMesh.size() * sizeof(float), &this->transparentMesh[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
	transparentMesh.clear(); // don't need after mesh is built
	this->setState(RENDER);
}

//can cut these down to one function by passing in the changes as variables

void Chunk::addFace(int face, int x, int y, int z, int val, vector<float> *m, int *ps)
{
	int xtype = (blocks[x][y][z].getType() - 1) % 16;
	int ytype = blocks[x][y][z].getType() / 17;
	for (int i = face * 6, j = 0; i < face * 6 + 6; j++, i++)
	{
		// vertices
		glm::vec3 vert(	VERTICES[INDICES[i]].x * 0.5f + (float)x,
						VERTICES[INDICES[i]].y * 0.5f + (float)y,
						VERTICES[INDICES[i]].z * 0.5f + (float)z);
		m->push_back(vert.x);
		m->push_back(vert.y);
		m->push_back(vert.z);

		// textures
		glm::vec2 tex(TEXCOORDS[j].x, TEXCOORDS[j].y);
		tex.x /= 16;

		// if (side && grass)
		// else if (top)
		// else
		if (blocks[x][y][z].getType() == GRASS_BLOCK && face != 1)
			tex.x += 0.0625 * (xtype+1);
		else if (blocks[x][y][z].getType() == TREE_BLOCK && (face == 1 || face == 0))
			tex.x += 0.0625 * (xtype+1);
		else
			tex.x += 0.0625 * xtype;
		
		// if (cactus and top)
		// else
		tex.y /= 16;
		if (blocks[x][y][z].getType() == CACTUS_BLOCK && (face == 1 || face == 0))
			tex.y += 0.0625 * (ytype+1);
		else
			tex.y += 0.0625 * ytype;
		m->push_back(tex.x);
		m->push_back(tex.y);

		// normals
		m->push_back(NORMALS[INDICES[i / 6]].x);
		m->push_back(NORMALS[INDICES[i / 6]].y);
		m->push_back(NORMALS[INDICES[i / 6]].z);

		// torch lighting, becomes a float when pushed back because mesh is a float vector
		m->push_back(this->torchLightMap[x][y][z]);

		// can move these light maps to private and use getTorchLight
		m->push_back(this->sunLightMap[x][y][z]);
	}
	*ps+=6;
}

void Chunk::cleanVAO(void) {
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->transparentVBO);
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteVertexArrays(1, &this->transparentVAO);
}



// Get the bits XXXX0000
// inline int Chunk::getSunlight(int x, int y, int z)
// {
// 	return (lightMap[x][y][z] >> 4) & 0xF;
// }

// // Set the bits XXXX0000
// inline void Chunk::setSunlight(int x, int y, int z, int val)
// {
// 	lightMap[x][y][z] = (lightMap[x][y][z] & 0xF) | (val << 4);
// }

// // Get the bits 0000XXXX
// inline int Chunk::getTorchlight(int x, int y, int z)
// {
// 	return lightMap[x][y][z] & 0xF;
// }
// // Set the bits 0000XXXX

// inline void Chunk::setTorchlight(int x, int y, int z, int val)
// {
// 	lightMap[x][y][z] = (lightMap[x][y][z] & 0xF0) | val;
// }