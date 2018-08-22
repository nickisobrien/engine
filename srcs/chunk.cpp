#include "engine.h"
#include "chunk.h"

Chunk::Chunk(int xoff, int zoff)
{
	this->xoff = xoff;
	this->zoff = zoff;
	blocks = new Block**[CHUNK_X];
	for(int i = 0; i < CHUNK_X; i++)
	{
		blocks[i] = new Block*[CHUNK_Y];

		for(int j = 0; j < CHUNK_Y; j++)
		{
			blocks[i][j] = new Block[CHUNK_Z];
		}
	}
	offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3((float)(xoff * CHUNK_X), 1.0f, (float)(zoff * CHUNK_Z)));
	offsetMatrix = glm::translate(offsetMatrix, glm::vec3(0.5f, -0.5f, 0.5f));
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

Chunk::~Chunk(void)
{
	for (int i = 0; i < CHUNK_X; ++i)
	{
		for (int j = 0; j < CHUNK_Y; ++j)
		{
			delete [] blocks[i][j];
		}
		delete [] blocks[i];
	}
	delete [] blocks;
	points.clear();
	uvs.clear();
	transparentPoints.clear();
	transparentUvs.clear();
}

void Chunk::render(Shader shader)
{
	shader.setMat4("transform", this->offsetMatrix);
	shader.setFloat("transparency", 1.0f);
	glBindVertexArray(VAO);
	int err = glGetError();
	if (err)
		cout << "ERROR " << err << endl;
	// cout << "size " << this->points.size() << endl;
	glDrawArrays(GL_TRIANGLES, 0, this->pointSize); //RIGHT HERE CAUSES THE SEGFAULTS
	// cout << "TEST2" << endl;
}

void Chunk::renderWater(Shader shader)
{
	shader.setMat4("transform", offsetMatrix);
	shader.setFloat("transparency", 0.85f);
	glBindVertexArray(transparentVAO);
	glDrawArrays(GL_TRIANGLES, 0, transparentPointSize);
}

void Chunk::setTerrain(FastNoise terrainNoise, FastNoise temperatureNoise, FastNoise humidityNoise)
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
			float b = MAP(terrainNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff)), -1.0f, 1.0f, 0.1f, 14.0f);
			int base = pow(b, 2);
			float temp = temperatureNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff));
			float hum = humidityNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff));
			short blocktype;
			// noise layer #1 "Temperature"
			// noise layer #2 "Humidity"
			// Derived biomes:
			// Temp < 33%	 		Humidity < 50% => Cold rocky biome
			// Temp < 33%	 		Humidity > 50% => Ice and frozen lakes
			// 33% < Temp < 66%	Humidity < 50% => Grassland
			// 33% < Temp < 66%	Humidity > 50% => Forest or swamp
			// Temp > 66%	 		Humidity < 50% => Dessert
			// Temp > 66%	 		Humidity > 50% => Tropical rainforest
			if (temp < -0.33f)
			{
				if (hum < 0.0f)
					blocktype = 67;
				else
					blocktype = 68;
			}
			else if (temp >= -0.33f && temp >= 0.33f)
			{
				if (hum < 0.0f)
					blocktype = 4;
				else
					blocktype = 3;
			}
			else
			{
				if (hum < 0.0f)
					blocktype = 19;
				else
					blocktype = 4;
			}
			if (base < WATER_LEVEL)
				water = true;
			for (int y = 0; y < base; y++)
			{
				this->blocks[x][y][z].setType(blocktype); // switched to grassland for now
			}
			for (int y = base; y < WATER_LEVEL; y++)
				this->blocks[x][y][z].setType(WATER_BLOCK);
		}
	}
	// this->addExtras(terrainNoise, temperatureNoise, humidityNoise);
	// std::cout << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << std::endl;
}

void Chunk::addExtras(FastNoise terrainNoise, FastNoise temperatureNoise, FastNoise humidityNoise)
{
	// trees
	Chunk *c = this;
	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int z = 0; z < CHUNK_Z; z++)
		{
			bool water = false;
			float b = MAP(terrainNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff)), -1.0f, 1.0f, 0.1f, 14.0f);
			int base = pow(b, 2);
			float temp = temperatureNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff));
			float hum = humidityNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff));
			short blocktype;
			int y = base;
			if (temp < -0.33f)
			{
				if (hum < 0.0f)
					blocktype = 67;
				else
					blocktype = 68;
			}
			else if (temp >= -0.33f && temp >= 0.33f)
			{
				if (hum < 0.0f)
					blocktype = 4;
				else
					blocktype = 3;
			}
			else
			{
				if (hum < 0.0f)
					blocktype = 19;
				else
					blocktype = 4;
			}
			if (base < WATER_LEVEL)
				water = true;
			if (!water && blocktype == 4 && rand() % 1000 > 996)
			{
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						for (int k = -1; k < 12; k++)
						{
							if (base+k < CHUNK_Y && base+k > 0)
							{
								if (x+i == CHUNK_X-1)
									if (c->getXPlus())
										c = c->getXPlus();
								if (z+j == CHUNK_Z-1)
								{
									if (c->getZPlus())
										c = c->getZPlus();
								}
								if (x+i >= CHUNK_X || z+j >= CHUNK_Z)
									continue;
								if (c->blocks[(x+i)%CHUNK_X][(base+k)%CHUNK_Y][(z+j)%CHUNK_Z].getType() == AIR_BLOCK)
								{
									c->blocks[(x+i)%CHUNK_X][(base+k)%CHUNK_Y][(z+j)%CHUNK_Z].setType(21);
								}
							}
						}
					}
				}
			} // cactus
			else if (!water && blocktype == 19 && rand() % 1000 > 996)
			{
				this->blocks[x][base][z].setType(71);
				this->blocks[x][base+1][z].setType(71);
				this->blocks[x][base+2][z].setType(71);
			}
		}
	}
}

void Chunk::update(void)
{
	this->cleanVAO();

	// for safety
	transparentPointSize = 0;
	pointSize = 0;

	this->faceRendering();
	this->buildVAO();
}

void Chunk::faceRendering(void)
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
				if (this->blocks[x][y][z].getType() == AIR_BLOCK)
				{
					this->blocks[x][y][z].setActive(false);
					continue ;
				}
				else if (this->blocks[x][y][z].getType() == WATER_BLOCK) //water_BLOCK
				{
					this->blocks[x][y][z].setActive(false);
					transparent = true;
				}
				int val = this->getWorld(x, y, z);

				// MINUS checks
				if (!x && this->xMinus)
					xMinusCheck = this->xMinus->blocks[CHUNK_X-1][y][z].getType();
				else if (!x)
					xMinusCheck = 1;
				else
					xMinusCheck = this->blocks[x-1][y][z].getType();

				if (!z && this->zMinus)
					zMinusCheck = this->zMinus->blocks[x][y][CHUNK_Z-1].getType();
				else if (!z)
					zMinusCheck = 1;
				else
					zMinusCheck = this->blocks[x][y][z-1].getType();

				if (!y)
					yMinusCheck = 1;
				 else
				 	yMinusCheck = blocks[x][y-1][z].getType();

				 // PLUS CHECKS
				if (x == CHUNK_X-1 && this->xPlus)
					xPlusCheck = this->xPlus->blocks[0][y][z].getType();
				else if (x == CHUNK_X-1)
					xPlusCheck = 1;
				else
					xPlusCheck = this->blocks[x+1][y][z].getType();

				if (z == CHUNK_Z-1 && this->zPlus)
					zPlusCheck = this->zPlus->blocks[x][y][0].getType();
				else if (z == CHUNK_Z-1)
					zPlusCheck = 1;
				else
					zPlusCheck = this->blocks[x][y][z+1].getType();

				if (y == CHUNK_Y-1)
					yPlusCheck = 1;
				else
					yPlusCheck = this->blocks[x][y+1][z].getType();

				// Facing
				if (!transparent)
				{
					if (yMinusCheck==AIR_BLOCK || yMinusCheck==WATER_BLOCK)
						this->addFace(0, x , y, z, val); //DOWN
					if (yPlusCheck==AIR_BLOCK || yPlusCheck==WATER_BLOCK)
						this->addFace(1, x , y, z, val); //UP
					if (xPlusCheck==AIR_BLOCK || xPlusCheck==WATER_BLOCK)
						this->addFace(2, x , y, z, val); //xpos SIDE
					if (zPlusCheck==AIR_BLOCK || zPlusCheck==WATER_BLOCK)
						this->addFace(3, x , y, z, val); //zpos SIDE
					if (xMinusCheck==AIR_BLOCK || xMinusCheck==WATER_BLOCK)
						this->addFace(4, x , y, z, val); //xneg SIDE
					if (zMinusCheck==AIR_BLOCK || zMinusCheck==WATER_BLOCK)
						this->addFace(5, x , y, z, val); //zneg SIDE
				}
				else
				{
					if (yMinusCheck==AIR_BLOCK || (yMinusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addTransparentFace(0, x , y, z, val); //DOWN
					if (yPlusCheck==AIR_BLOCK || (yPlusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addTransparentFace(1, x , y, z, val); //UP
					if (xPlusCheck==AIR_BLOCK || (xPlusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addTransparentFace(2, x , y, z, val); //xpos SIDE
					if (zPlusCheck==AIR_BLOCK || (zPlusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addTransparentFace(3, x , y, z, val); //zpos SIDE
					if (xMinusCheck==AIR_BLOCK || (xMinusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addTransparentFace(4, x , y, z, val); //xneg SIDE
					if (zMinusCheck==AIR_BLOCK || (zMinusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addTransparentFace(5, x , y, z, val); //zneg SIDE
				}
			}
		}
	}
}

void Chunk::buildVAO(void)
{
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	// vertice VBO
	glGenBuffers(1, &this->VBO_VERT);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_VERT);
	glBufferData(GL_ARRAY_BUFFER, this->points.size() * sizeof(glm::vec3), &this->points[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	this->pointSize = this->points.size();
	this->points.clear();

	// texture coords
	glGenBuffers(1, &this->VBO_UV);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_UV);
	glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glEnableVertexAttribArray(1);

	this->uvs.clear();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// TRANSPARENT
	glGenVertexArrays(1, &this->transparentVAO);
	glBindVertexArray(this->transparentVAO);

	// vertice VBO
	glGenBuffers(1, &this->transparentVBO_VERT);
	glBindBuffer(GL_ARRAY_BUFFER, this->transparentVBO_VERT);
	glBufferData(GL_ARRAY_BUFFER, this->transparentPoints.size() * sizeof(glm::vec3), &this->transparentPoints[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	this->transparentPointSize = this->transparentPoints.size();
	this->transparentPoints.clear();

	// texture coords
	glGenBuffers(1, &this->transparentVBO_UV);
	glBindBuffer(GL_ARRAY_BUFFER, this->transparentVBO_UV);
	glBufferData(GL_ARRAY_BUFFER, this->transparentUvs.size() * sizeof(glm::vec2), &this->transparentUvs[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glEnableVertexAttribArray(1);

	this->transparentUvs.clear();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Chunk::addFace(int face, int x, int y, int z, int val)
{
	static int oneFace = ((sizeof(VCUBE) / 4)/6/2);
	static int oneFaceUV = ((sizeof(CUBEUV) / 4)/6/2);
	int u = face + 1;

	for (int i = oneFace * face; i < oneFace * u; i+=3)
	{
		glm::vec3 vec = glm::make_vec3(&VCUBE[i]);
		vec.x = vec.x * 0.5f + (float)x;
		vec.y = vec.y * 0.5f + (float)y;
		vec.z = vec.z * 0.5f + (float)z;
		points.push_back(vec);
	}

	for (int i = oneFace * face + 54; i < oneFace * u + 54; i+=3)
	{
		glm::vec3 vec = glm::make_vec3(&VCUBE[i]);
		vec.x = vec.x * 0.5f + (float)x;
		vec.y = vec.y * 0.5f + (float)y;
		vec.z = vec.z * 0.5f + (float)z;
		points.push_back(vec);
	}

	int xtype = blocks[x][y][z].getType() - 1 % 16;
	int ytype = blocks[x][y][z].getType() / 17;
	for (int i = oneFaceUV * face; i < oneFaceUV * u; i += 2)
	{
		glm::vec2 vec = glm::make_vec2(&CUBEUV[i]);
		vec.x /= 16;
		vec.x += 0.0625 * (xtype);
		vec.y /= 16;
		vec.y += 0.0625 * (ytype);
		uvs.push_back(vec);
	}

	for (int i = oneFaceUV * face + 36; i < oneFaceUV * u + 36; i+=2)
	{
		glm::vec2 vec = glm::make_vec2(&CUBEUV[i]);
		vec.x /= 16;
		vec.x += 0.0625 * (xtype);
		vec.y /= 16;
		vec.y += 0.0625 * (ytype);
		uvs.push_back(vec);
	}
}

void Chunk::addTransparentFace(int face, int x, int y, int z, int val)
{
	static int oneFace = ((sizeof(VCUBE) / 4)/6/2);
	static int oneFaceUV = ((sizeof(CUBEUV) / 4)/6/2);
	int u = face + 1;

	for (int i = oneFace * face; i < oneFace * u; i+=3)
	{
		glm::vec3 vec = glm::make_vec3(&VCUBE[i]);
		vec.x = vec.x * 0.5f + (float)x;
		vec.y = vec.y * 0.5f + (float)y;
		vec.z = vec.z * 0.5f + (float)z;
		transparentPoints.push_back(vec);
	}

	for (int i = oneFace * face + 54; i < oneFace * u + 54; i+=3)
	{
		glm::vec3 vec = glm::make_vec3(&VCUBE[i]);
		vec.x = vec.x * 0.5f + (float)x;
		vec.y = vec.y * 0.5f + (float)y;
		vec.z = vec.z * 0.5f + (float)z;
		transparentPoints.push_back(vec);
	}

	int xtype = blocks[x][y][z].getType() - 1 % 16;
	int ytype = blocks[x][y][z].getType() / 17;
	for (int i = oneFaceUV * face; i < oneFaceUV * u; i += 2)
	{
		glm::vec2 vec = glm::make_vec2(&CUBEUV[i]);
		vec.x /= 16;
		vec.x += 0.0625 * (xtype);
		vec.y /= 16;
		vec.y += 0.0625 * (ytype);
		transparentUvs.push_back(vec);
	}

	for (int i = oneFaceUV * face + 36; i < oneFaceUV * u + 36; i+=2)
	{
		glm::vec2 vec = glm::make_vec2(&CUBEUV[i]);
		vec.x /= 16;
		vec.x += 0.0625 * (xtype);
		vec.y /= 16;
		vec.y += 0.0625 * (ytype);
		transparentUvs.push_back(vec);
	}
}

void Chunk::cleanVAO(void) {
	glDeleteBuffers(1, &this->VBO_UV);
	glDeleteBuffers(1, &this->VBO_VERT);
	glDeleteVertexArrays(1, &this->VAO);

	glDeleteBuffers(1, &this->transparentVBO_UV);
	glDeleteBuffers(1, &this->transparentVBO_VERT);
	glDeleteVertexArrays(1, &this->transparentVAO);
}

void Chunk::setXMinus(Chunk *chunk)
{
	this->xMinus = chunk;
}
void Chunk::setXPlus(Chunk *chunk)
{
	this->xPlus = chunk;
}
void Chunk::setZMinus(Chunk *chunk)
{
	this->zMinus = chunk;
}
void Chunk::setZPlus(Chunk *chunk)
{
	this->zPlus = chunk;
}

Chunk *Chunk::getXMinus()
{
	return (this->xMinus);
}
Chunk *Chunk::getXPlus()
{
	return (this->xPlus);
}
Chunk *Chunk::getZMinus()
{
	return (this->zMinus);
}
Chunk *Chunk::getZPlus()
{
	return (this->zPlus);
}
