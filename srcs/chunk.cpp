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

Block *Chunk::get_block(int x, int y, int z)
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
}

void Chunk::render(Shader shader)
{
	shader.setMat4("transform", offsetMatrix);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, points.size());
}

void Chunk::set_terrain(FastNoise myNoise)
{
	// prob should move the noise type to terrain and pass it in here
	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int z = 0; z < CHUNK_Z; z++)
		{
			// Use the noise library to get the height value of x, z
			// float height = MAP(noise((float)x/1000.0f, (float)z/1000.0f), -1.0f, 1.0f, 5, CHUNK_Y-1);
			int height = MAP(myNoise.GetNoise(x+(CHUNK_X*xoff),z+(CHUNK_Z*zoff)), -1.0f, 1.0f, 1.0f, CHUNK_Y-1);
			for (int y = 0; y < height; y++)
			{
				if (y < 60)
					this->blocks[x][y][z].setType(2);
				else if (y < 80)
					this->blocks[x][y][z].setType(1);
				else
					this->blocks[x][y][z].setType(3);
			}
			for (int y = height; y < 52; y++)
				this->blocks[x][y][z].setType(4);
		}
	}
}

void Chunk::update(void)
{
	cleanVAO();
	points.clear();
	glm::mat4 transform = this->offsetMatrix;

	this->faceRendering();
	buildVAO();
}

void Chunk::faceRendering(void)
{
	int xMinusCheck;
	int yMinusCheck;
	int zMinusCheck;
	int xPlusCheck;
	int yPlusCheck;
	int zPlusCheck;
	for(int x = 0; x < CHUNK_X; x++)
	{
		for(int y = 0; y < CHUNK_Y; y++) // NOTE: not rendering floor atm, can change back to y = 1 after TODO vvv
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				if (this->blocks[x][y][z].getType() == 0)
				{
					this->blocks[x][y][z].setActive(false);
					continue ;
				}
				else if (this->blocks[x][y][z].getType() == 4)
					this->blocks[x][y][z].setActive(false);
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

				// Deactivating hidden blocks
				if (xMinusCheck==0 && yMinusCheck==0 && zMinusCheck==0 && xPlusCheck==0 && yPlusCheck==0 && zPlusCheck==0)
				{
					this->blocks[x][y][z].setActive(false);
					continue ;
				}
				
				// Facing
				if (yMinusCheck==0)// || yMinusCheck==4)
					this->add_face(0, x , y, z, val); //DOWN
				if (yPlusCheck==0)// || yPlusCheck==4)
					this->add_face(1, x , y, z, val); //UP
				if (xPlusCheck==0)// || xPlusCheck==4)
					this->add_face(2, x , y, z, val); //xpos SIDE
				if (xMinusCheck==0)// || xMinusCheck==4)
					this->add_face(4, x , y, z, val); //xneg SIDE
				if (zMinusCheck==0)// || zMinusCheck==4)
					this->add_face(5, x , y, z, val); //zneg SIDE
				if (zPlusCheck==0)// || zPlusCheck==4)
					this->add_face(3, x , y, z, val); //zpos SIDE
			}
		}
	}
}

float *Chunk::getVertices(void)
{
	return (&this->points[0][0]);
}

float *Chunk::getUVs(void)
{
	return (&this->uvs[0][0]);
}

void Chunk::buildVAO(void)
{
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	// vertice VBO
	glGenBuffers(1, &this->VBO_VERT);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_VERT);
	glBufferData(GL_ARRAY_BUFFER, this->points.size() * sizeof(glm::vec3), this->getVertices(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// texture coords
	glGenBuffers(1, &this->VBO_UV);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_UV);
	glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec3), this->getUVs(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glm::vec3 Chunk::getUVBlock(int val, int i, int face)
{
	glm::vec2 vec = glm::make_vec2(&CUBEUV[i]);
	float type;
	if (face == 0 || face == 1)
		type = 0.1f;
	else
		type = 1.1;
	glm::vec3 f = glm::vec3(vec.x, vec.y, type);
	return (f);
}


void Chunk::add_face(int face, int x, int y, int z, int val)
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

	for (int i = oneFaceUV * face; i < oneFaceUV * u; i += 2)
	{
		glm::vec3 vec = this->getUVBlock(val, i, face);
		vec.z = blocks[x][y][z].getType();
		// cout << vec.z << endl;
		uvs.push_back(vec);
	}

	for (int i = oneFaceUV * face + 36; i < oneFaceUV * u + 36; i+=2)
	{
		glm::vec3 vec = this->getUVBlock(val, i, face);
		vec.z = blocks[x][y][z].getType();
		// cout << vec.z << endl;
		uvs.push_back(vec);
	}
}

void Chunk::cleanVAO(void) {
	glDeleteBuffers(1, &this->VBO_VERT);
	glDeleteVertexArrays(1, &this->VAO);
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
