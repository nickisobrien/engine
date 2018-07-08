#include "engine.h"
#include "chunk.h"

Chunk::Chunk(int xoff, int zoff)
{
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
}

Chunk::~Chunk(void)
{
	// for (int i = 0; i < CHUNK_X; ++i)
	// {
	// 	for (int j = 0; j < CHUNK_Y; ++j)
	// 	{
	// 		delete [] blocks[i][j];
	// 	}

	// 	delete [] blocks[i];
	// }
	// delete [] blocks;
}

void Chunk::render(Shader shader)
{
	shader.setMat4("transform", offsetMatrix);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, points.size());
}

void Chunk::set_terrain(void)
{
	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int z = 0; z < CHUNK_Z; z++)
		{
			int num = round(rand() % 2);
			if (num > 0)
				blocks[x][CHUNK_Y-1][z].setType(1);
			else
				blocks[x][CHUNK_Y-1][z].setType(0);
		}
	}
}

void Chunk::update(void)
{
	cleanVAO();
	points.clear();
	for(int x = 0; x < CHUNK_X; x++)
	{
		for(int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				if (touchingAir(x, y, z) == true) // change this to which faces need rendering
					blocks[x][y][z].setActive(false);
			}
		}
	}

	glm::mat4 transform = offsetMatrix;
	for(int x = 0; x < CHUNK_X; x++)
	{
		for(int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				if (blocks[x][y][z].isActive() == true) // should add variables to block saying which sides are active
					this->add_face(0, x , y, z); //DOWN
				if (blocks[x][y][z].isActive() == true)
					this->add_face(1, x , y, z); //UP
				if (blocks[x][y][z].isActive() == true)
					this->add_face(2, x , y, z); //SIDE
				if (blocks[x][y][z].isActive() == true)
					this->add_face(3, x , y, z); //SIDE
				if (blocks[x][y][z].isActive() == true)
					this->add_face(4, x , y, z); //SIDE
				if (blocks[x][y][z].isActive() == true)
					this->add_face(5, x , y, z); //SIDE
			}
		}
	}
	buildVAO();
}

bool Chunk::touchingAir(int x, int y, int z)
{
	if (blocks[x][y][z].getType() == 0)
		return (true);
	if (!x || !z  || !y || z == CHUNK_Z - 1 || x == CHUNK_X - 1 || y == CHUNK_Y - 1)
		return (false);
	else if (blocks[x+1][y][z].getType() == 0 || blocks[x-1][y][z].getType() == 0 ||
		blocks[x][y+1][z].getType() == 0 || blocks[x][y-1][z].getType() == 0 ||
		blocks[x][y][z+1].getType() == 0 || blocks[x][y][z-1].getType() == 0)
		return (false);
	return (true);
}

float *Chunk::getVertices(void)
{
	return (&this->points[0][0]);
}

size_t Chunk::getSizeVertices(void)
{
	return (this->points.size() * sizeof(glm::vec3));
}

void Chunk::buildVAO(void)
{
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	// vertice VBO
	glGenBuffers(1, &this->VBO_VERT);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_VERT);
	glBufferData(GL_ARRAY_BUFFER, this->getSizeVertices(), this->getVertices(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Chunk::add_face(int face, int x, int y, int z)
{
	static int oneFace = ((sizeof(VCUBE) / 4)/6/2);
	int u = face + 1;

	for (int i = oneFace * face; i < oneFace * u; i+=3)
	{
		glm::vec3 vec = glm::make_vec3(&VCUBE[i]);
		vec.x = vec.x*0.5f + (float)x;
		vec.y = vec.y*0.5f +(float)y;
		vec.z = vec.z*0.5f +(float)z;
		points.push_back(vec);
	}

	for (int i = oneFace * face + 54; i < oneFace * u + 54; i+=3)
	{
		glm::vec3 vec = glm::make_vec3(&VCUBE[i]);
		vec.x = vec.x*0.5f + (float)x*1;
		vec.y = vec.y*0.5f +(float)y*1;
		vec.z = vec.z*0.5f +(float)z*1;
		points.push_back(vec);
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
