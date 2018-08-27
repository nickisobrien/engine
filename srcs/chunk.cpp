#include "engine.h"
#include "chunk.h"

#define YSQRT sqrt(CHUNK_Y-1)

Chunk::Chunk(int xoff, int zoff)
{
	this->xoff = xoff;
	this->zoff = zoff;
	offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3((float)(xoff * CHUNK_X), 1.0f, (float)(zoff * CHUNK_Z)));
	offsetMatrix = glm::translate(offsetMatrix, glm::vec3(0.5f, -0.5f, 0.5f));

	// zero lightmap
	memset(lightMap, 0, sizeof(lightMap));

	// non transparent
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
		// glGenBuffers(1, &this->VBO_UV);
		// glGenBuffers(1, &this->VBO_NORM);
		
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		// glEnableVertexAttribArray(0);

		// // glBindBuffer(GL_ARRAY_BUFFER, this->VBO_UV);
		// // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(0));
		// // glEnableVertexAttribArray(1);

		// // glBindBuffer(GL_ARRAY_BUFFER, this->VBO_NORM);
		// // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(0));
		// // glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		
		// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		// glEnableVertexAttribArray(1);
		
		// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		// glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// transparent
	// glGenVertexArrays(1, &this->transparentVAO);
	// 	glGenBuffers(1, &this->transparentVBO_VERT);
	// 	glGenBuffers(1, &this->transparentVBO_UV);
	// 	glBindVertexArray(this->transparentVAO);

	// 	glBindBuffer(GL_ARRAY_BUFFER, this->transparentVBO_VERT);
	// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// 	glEnableVertexAttribArray(0);

	// 	glBindBuffer(GL_ARRAY_BUFFER, this->transparentVBO_UV);
	// 	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	// 	glEnableVertexAttribArray(1);

	// 	glBindBuffer(GL_ARRAY_BUFFER, 0); // don't think i need
	// glBindVertexArray(0);
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
	// points.clear();
	// uvs.clear();
	// transparentPoints.clear();
	// transparentUvs.clear();
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
			float b = MAP(terrainNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff)), -1.0f, 1.0f, 0.1f, YSQRT);
			int base = pow(b, 2);
			float temp = temperatureNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff));
			float hum = humidityNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff));
			short blocktype;
			// noise layer #1 "Temperature"
			// noise layer #2 "Humidity"
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
					blocktype = GRASS_BLOCK;
				else
					blocktype = DIRT_BLOCK;
			}
			else
			{
				if (hum < 0.0f)
					blocktype = SAND_BLOCK;
				else
					blocktype = GRASS_BLOCK;
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
	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int z = 0; z < CHUNK_Z; z++)
		{
			// bool water = false;
			float b = MAP(terrainNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff)), -1.0f, 1.0f, 0.1f, YSQRT);
			int base = pow(b, 2);
			float temp = temperatureNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff));
			float hum = humidityNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff));
			short blocktype;
			int y = base;
			Chunk *c = this;
			if (temp < -0.33f)
			{
				if (hum < 0.0f)
					blocktype = 67;
				else
					blocktype = SNOW_BLOCK;
			}
			else if (temp >= -0.33f && temp >= 0.33f)
			{
				if (hum < 0.0f)
					blocktype = GRASS_BLOCK;
				else
					blocktype = DIRT_BLOCK;
			}
			else
			{
				if (hum < 0.0f)
					blocktype = SAND_BLOCK;
				else
					blocktype = GRASS_BLOCK;
			}
			if (base < WATER_LEVEL)
				continue ;
			if (blocktype == GRASS_BLOCK && rand() % 1000 > 996)
			{
				this->blocks[x][base][z].setType(DIRT_BLOCK);
				this->blocks[x][base+1][z].setType(DIRT_BLOCK);
				this->blocks[x][base+2][z].setType(DIRT_BLOCK);
				this->blocks[x][base+3][z].setType(DIRT_BLOCK);
				this->blocks[x][base+4][z].setType(DIRT_BLOCK);
				this->blocks[x][base+5][z].setType(DIRT_BLOCK);
				this->blocks[x][base+6][z].setType(DIRT_BLOCK);

				this->blocks[x][base][z].setActive(true); // why not happening on update?
				this->blocks[x][base+1][z].setActive(true); // why not happening on update?
				this->blocks[x][base+2][z].setActive(true); // why not happening on update?
				this->blocks[x][base+3][z].setActive(true); // why not happening on update?
				this->blocks[x][base+4][z].setActive(true); // why not happening on update?
				this->blocks[x][base+5][z].setActive(true); // why not happening on update?
				this->blocks[x][base+6][z].setActive(true); // why not happening on update?
				// for (int i = 0; i < 2; i++)
				// {
				// 	for (int j = 0; j < 2; j++)
				// 	{
				// 		for (int k = -1; k < 12; k++)
				// 		{
				// 			if (base+k < CHUNK_Y && base+k > 0)
				// 			{
				// 				if (x+i == CHUNK_X)
				// 					if (c->getXPlus())
				// 						c = c->getXPlus();
				// 				if (z+j == CHUNK_Z)
				// 				{
				// 					if (c->getZPlus())
				// 						c = c->getZPlus();
				// 				}
				// 				if (c->blocks[(x+i)%CHUNK_X][(base+k)%CHUNK_Y][(z+j)%CHUNK_Z].getType() == AIR_BLOCK)
				// 				{
				// 					c->blocks[(x+i)%CHUNK_X][(base+k)%CHUNK_Y][(z+j)%CHUNK_Z].setType(21);
				// 				}
				// 			}
				// 		}
				// 	}
				// }
			} // cactus
			else if (blocktype == SAND_BLOCK && rand() % 1000 > 996)
			{
				this->blocks[x][base][z].setType(GRASS_BLOCK);
				this->blocks[x][base+1][z].setType(GRASS_BLOCK);
				this->blocks[x][base+2][z].setType(GRASS_BLOCK);

				this->blocks[x][base][z].setActive(true); // why not happening on update?
				this->blocks[x][base+1][z].setActive(true); // why not happening on update?
				this->blocks[x][base+2][z].setActive(true); // why not happening on update?
			}
		}
	}
}

void Chunk::update(void)
{
	this->cleanVAO();

	mesh.clear();
	// for safety
	transparentPointSize = 0;
	pointSize = 0;

	this->faceRendering();
	this->buildVAO();
}

void Chunk::faceRendering(void)
{
	// update is called when all neighbors are set, is an okay idea because of the addextras call happens too
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
				else if (!x) // if x == 0 and there isn't an xminus neighbor, infer that we don't need to draw it
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
	// vertice VBO
	glBindVertexArray(this->VAO);

		// glBindBuffer(GL_ARRAY_BUFFER, this->VBO_VERT);
		// glBufferData(GL_ARRAY_BUFFER, this->points.size() * sizeof(glm::vec3), &this->points[0][0], GL_STATIC_DRAW);

		// // texture coords
		// glBindBuffer(GL_ARRAY_BUFFER, this->VBO_UV);
		// glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0][0], GL_STATIC_DRAW);

		// glBindBuffer(GL_ARRAY_BUFFER, this->VBO_NORM);
		// glBufferData(GL_ARRAY_BUFFER, this->norms.size() * sizeof(glm::vec2), &this->norms[0][0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), &this->mesh[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	// mesh.clear();

	// this->points.clear();
	// this->uvs.clear();
	// this->norms.clear();


	// TRANSPARENT
	// glBindVertexArray(this->transparentVAO);

	// 	glBindBuffer(GL_ARRAY_BUFFER, this->transparentVBO_VERT);
	// 	glBufferData(GL_ARRAY_BUFFER, this->transparentPoints.size() * sizeof(glm::vec3), &this->transparentPoints[0][0], GL_STATIC_DRAW);
	
	// 	// texture coords
	// 	glBindBuffer(GL_ARRAY_BUFFER, this->transparentVBO_UV);
	// 	glBufferData(GL_ARRAY_BUFFER, this->transparentUvs.size() * sizeof(glm::vec2), &this->transparentUvs[0][0], GL_STATIC_DRAW);

	// glBindVertexArray(0);

	// this->transparentPointSize = this->transparentPoints.size();
	// this->transparentPoints.clear();
	// this->transparentUvs.clear();










	// // vertice VBO
	// 
	// 
	// glBufferData(GL_ARRAY_BUFFER, this->transparentPoints.size() * sizeof(glm::vec3), &this->transparentPoints[0][0], GL_STATIC_DRAW);
	// 
	// 

	// this->transparentPointSize = this->transparentPoints.size();
	// this->transparentPoints.clear();

	// // texture coords
	// 
	// 
	// glBufferData(GL_ARRAY_BUFFER, this->transparentUvs.size() * sizeof(glm::vec2), &this->transparentUvs[0][0], GL_STATIC_DRAW);
	// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	// glEnableVertexAttribArray(1);

	// this->transparentUvs.clear();

	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);
}

//can cut these down to one function by oassing in the changes as variables

void Chunk::addFace(int face, int x, int y, int z, int val)
{
	for (int i = face * 6; i < face * 6 + 6; i++)
	{
		glm::vec3 vec(	vertices[indices[i]].x * 0.5f + (float)x,
						vertices[indices[i]].y * 0.5f + (float)y,
						vertices[indices[i]].z * 0.5f + (float)z);
		// mesh.push_back(vec);
		mesh.push_back(vec.x);
		mesh.push_back(vec.y);
		mesh.push_back(vec.z);
	}
	this->pointSize+=6;

	// int xtype = blocks[x][y][z].getType() - 1 % 16;
	// int ytype = blocks[x][y][z].getType() / 17;
	// for (int i = 0; i < 6; i++)
	// {
	// 	glm::vec2 vec(texCoords[texInds[i % 4]].x, texCoords[texInds[i % 4]].y);
	// 	vec.x /= 16;
	// 	vec.x += 0.0625 * (xtype);
	// 	vec.y /= 16;
	// 	vec.y += 0.0625 * (ytype);
	// 	mesh.push_back(vec.x);
	// 	mesh.push_back(vec.y);
	// }

	// for (int i = face * 6; i < face * 6 + 6; i++)
	// {
	// 	glm::vec3 vec(	normals[indices[i / 6]].x,
	// 					normals[indices[i / 6]].y,
	// 					normals[indices[i / 6]].z);
	// 	mesh.push_back(vec.x);
	// 	mesh.push_back(vec.y);
	// 	mesh.push_back(vec.z);
	// 	// norms.push_back(vec);
	// }
}

void Chunk::addTransparentFace(int face, int x, int y, int z, int val)
{
	for (int i = face * 6; i < face * 6 + 6; i++)
	{
		glm::vec3 vec(	vertices[indices[i]].x * 0.5f + (float)x,
						vertices[indices[i]].y * 0.5f + (float)y,
						vertices[indices[i]].z * 0.5f + (float)z);
		transparentPoints.push_back(vec);
	}

	int xtype = blocks[x][y][z].getType() - 1 % 16;
	int ytype = blocks[x][y][z].getType() / 17;
	for (int i = 0; i < 6; i++)
	{
		glm::vec2 vec(texCoords[texInds[i % 4]].x, texCoords[texInds[i % 4]].y);
		vec.x /= 16;
		vec.x += 0.0625 * (xtype);
		vec.y /= 16;
		vec.y += 0.0625 * (ytype);
		transparentUvs.push_back(vec);
	}

	for (int i = face * 6; i < face * 6 + 6; i++)
	{
		glm::vec3 vec(	normals[indices[i / 6]].x,
						normals[indices[i / 6]].y,
						normals[indices[i / 6]].z);
		transparentNorms.push_back(vec);
	}
}

void Chunk::cleanVAO(void) {
	// glDeleteBuffers(1, &this->VBO_UV);
	// glDeleteBuffers(1, &this->VBO_VERT);
	// glDeleteVertexArrays(1, &this->VAO);

	// glDeleteBuffers(1, &this->transparentVBO_UV);
	// glDeleteBuffers(1, &this->transparentVBO_VERT);
	// glDeleteVertexArrays(1, &this->transparentVAO);
}


// Get the bits XXXX0000
inline int Chunk::getSunlight(int x, int y, int z)
{
	return (lightMap[x][y][z] >> 4) & 0xF;
}

// Set the bits XXXX0000
inline void Chunk::setSunlight(int x, int y, int z, int val)
{
	lightMap[x][y][z] = (lightMap[x][y][z] & 0xF) | (val << 4);
}

// Get the bits 0000XXXX
inline int Chunk::getTorchlight(int x, int y, int z)
{
	return lightMap[x][y][z] & 0xF;
}
// Set the bits 0000XXXX

inline void Chunk::setTorchlight(int x, int y, int z, int val)
{
	lightMap[x][y][z] = (lightMap[x][y][z] & 0xF0) | val;
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
