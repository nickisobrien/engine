#include "engine.h"
#include "chunk.h"

#define YSQRT  sqrt(CHUNK_Y-1)

Chunk::Chunk(int x, int z) : xoff(x), zoff(z)
{
	this->pointSize = 0;
	this->transparentPointSize = 0;
	// this->xoff = xoff;
	// this->zoff = zoff;
	offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3((float)(xoff * CHUNK_X), 1.0f, (float)(zoff * CHUNK_Z)));
	offsetMatrix = glm::translate(offsetMatrix, glm::vec3(0.5f, -0.5f, 0.5f));

	// zero lightmap
	memset(torchLightMap, 0, sizeof(torchLightMap));
	memset(sunLightMap, 0, sizeof(sunLightMap));

	// non transparent
	glGenVertexArrays(1, &this->VAO);
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

Chunk::~Chunk(void)
{
	this->cleanVAO();
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
			for (int y = 0; y < base - 4; y++)
			{
				this->blocks[x][y][z].setType(STONE_BLOCK);
			}
			for (int y = base - 4; y < base; y++)
			{
				this->blocks[x][y][z].setType(blocktype); // switched to grassland for now
			}
			for (int y = base; y < WATER_LEVEL; y++)
			{
				water = true;
				this->blocks[x][y][z].setType(WATER_BLOCK);
			}


			// extras
			if (!water)
			{

				if (blocktype == GRASS_BLOCK && x > 2 && z > 2 && x + 2 < CHUNK_X && z + 2 < CHUNK_Z && rand() % 1000 > 996)
				{
					// trunk
					for (int y = 0; y < 6; y++)
					{
						this->blocks[x][base+y][z].setType(DIRT_BLOCK);
					}
					for (int i = -2; i <= 2; i++)
					{
						for (int j = -2; j <= 2; j++)
						{
							this->blocks[x+i][base+6][z+j].setType(GRASS_BLOCK);
						}
					}
					for (int i = -1; i <= 1; i++)
					{
						for (int j = -1; j <= 1; j++)
						{
							this->blocks[x+i][base+7][z+j].setType(GRASS_BLOCK);
						}
					}
				} // cactus
				else if (blocktype == SAND_BLOCK && rand() % 1000 > 996)
				{
					for (int y = 0; y < 3; y++)
					{
						this->blocks[x][base+y][z].setType(GRASS_BLOCK);
					}
				}
			}

		}
	}
	// std::cout << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << std::endl;
}

void Chunk::update()
{
	this->built = false;
	this->transparentPointSize = 0;
	this->pointSize = 0;

	this->faceRendering();
	this->buildVAO();
	this->built = true;
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
				if (this->blocks[x][y][z].getType() == AIR_BLOCK)
					continue ;
				if (this->blocks[x][y][z].getType() == WATER_BLOCK) //water_BLOCK
					transparent = true;
				int val = this->getWorld(x, y, z);

				// MINUS checks
				if (!x && this->getXMinus())
					xMinusCheck = this->xMinus->blocks[CHUNK_X-1][y][z].getType();
				else
					xMinusCheck = this->blocks[x-1][y][z].getType();

				if (!z && this->getZMinus())
					zMinusCheck = this->zMinus->blocks[x][y][CHUNK_Z-1].getType();
				else
					zMinusCheck = this->blocks[x][y][z-1].getType();

				if (!y)
					yMinusCheck = 1;
				 else
				 	yMinusCheck = blocks[x][y-1][z].getType();

				 // PLUS CHECKS
				if (x == CHUNK_X-1 && this->getXPlus())
					xPlusCheck = this->xPlus->blocks[0][y][z].getType();
				else
					xPlusCheck = this->blocks[x+1][y][z].getType();

				if (z == CHUNK_Z-1 && this->getZPlus())
					zPlusCheck = this->zPlus->blocks[x][y][0].getType();
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
						this->addFace(0, x , y, z, val, &this->mesh, &this->pointSize); //DOWN
					if (yPlusCheck==AIR_BLOCK || yPlusCheck==WATER_BLOCK)
						this->addFace(1, x , y, z, val, &this->mesh, &this->pointSize); //UP
					if (xPlusCheck==AIR_BLOCK || xPlusCheck==WATER_BLOCK)
						this->addFace(2, x , y, z, val, &this->mesh, &this->pointSize); //xpos SIDE
					if (zPlusCheck==AIR_BLOCK || zPlusCheck==WATER_BLOCK)
						this->addFace(3, x , y, z, val, &this->mesh, &this->pointSize); //zpos SIDE
					if (xMinusCheck==AIR_BLOCK || xMinusCheck==WATER_BLOCK)
						this->addFace(4, x , y, z, val, &this->mesh, &this->pointSize); //xneg SIDE
					if (zMinusCheck==AIR_BLOCK || zMinusCheck==WATER_BLOCK)
						this->addFace(5, x , y, z, val, &this->mesh, &this->pointSize); //zneg SIDE
				}
				else
				{
					if (yMinusCheck==AIR_BLOCK || (yMinusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addFace(0, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //DOWN
					if (yPlusCheck==AIR_BLOCK || (yPlusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addFace(1, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //UP
					if (xPlusCheck==AIR_BLOCK || (xPlusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addFace(2, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //xpos SIDE
					if (zPlusCheck==AIR_BLOCK || (zPlusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addFace(3, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //zpos SIDE
					if (xMinusCheck==AIR_BLOCK || (xMinusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addFace(4, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //xneg SIDE
					if (zMinusCheck==AIR_BLOCK || (zMinusCheck==WATER_BLOCK && this->blocks[x][y][z].getType() != WATER_BLOCK))
						this->addFace(5, x , y, z, val, &this->transparentMesh, &this->transparentPointSize); //zneg SIDE
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
	mesh.clear();

	glBindVertexArray(this->transparentVAO);

		glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
		glBufferData(GL_ARRAY_BUFFER, transparentMesh.size() * sizeof(float), &this->transparentMesh[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	transparentMesh.clear();
}

//can cut these down to one function by passing in the changes as variables

void Chunk::addFace(int face, int x, int y, int z, int val, vector<float> *m, int *ps)
{
	int xtype = blocks[x][y][z].getType() - 1 % 16;
	int ytype = blocks[x][y][z].getType() / 17;
	for (int i = face * 6, j = 0; i < face * 6 + 6; j++, i++)
	{
		// vertices
		glm::vec3 vec1(	vertices[indices[i]].x * 0.5f + (float)x,
						vertices[indices[i]].y * 0.5f + (float)y,
						vertices[indices[i]].z * 0.5f + (float)z);
		m->push_back(vec1.x);
		m->push_back(vec1.y);
		m->push_back(vec1.z);

		// textures
		glm::vec2 vec2(texCoords[texInds[j % 4]].x, texCoords[texInds[j % 4]].y);
		vec2.x /= 16;
		vec2.x += 0.0625 * (xtype);
		vec2.y /= 16;
		vec2.y += 0.0625 * (ytype);
		m->push_back(vec2.x);
		m->push_back(vec2.y);

		// normals
		m->push_back(normals[indices[i / 6]].x);
		m->push_back(normals[indices[i / 6]].y);
		m->push_back(normals[indices[i / 6]].z);

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