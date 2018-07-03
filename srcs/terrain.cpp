#include "engine.h"
#include "terrain.h"

Terrain::Terrain(void)
{
	cols = 25;
	rows = 25;
	vertice_count = rows * cols * 2;
}

Terrain::Terrain(int c, int r)
{
	cols = c;
	rows = r;
	vertice_count = rows * cols * 2;
}

void Terrain::generate_coordinates(void)
{
	scl = 3.0f;
	float terr[rows][cols];

	float yoff = 0.0f;
	for (int y = 0; y < rows; y++)
	{
		float xoff = 0.0f;
		for (int x = 0; x < cols; x++)
		{
			terr[x][y] = MAP(noise(xoff, yoff),-1,1,-12,12);
			xoff += 0.1f;
		}
		yoff += 0.1f;
	}

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols - 1/* idk.. (something is weird with terr and this+ 1 */; x++)
		{
			vectvert.push_back(glm::vec3(x * scl, y * scl, terr[x][y])); // even row
			vectvert.push_back(glm::vec3(x * scl, (y + 1.0f) * scl, terr[x][y+1])); // odd row
		}
	}
	vertice_count = vectvert.size();
}

void Terrain::generate_blocks(void)
{
	for (int i = 0; i < vertice_count; i++)
	{
		vertices[i] = vectvert[i];
	}
}

void Terrain::bind_neighbors(int x, int z)
{
	int index = -1;
	for (int i = 0; i < chunks.size(); i++)
	{
		if (chunks[i].xoff == x && chunks[i].zoff == z)
		{
			index = i;
			break;
		}
	}
	if (chunks[index].mxneighbor == NULL)
	{
		Chunk c;
		c.xoff = x-1;
		c.zoff = z;
		c.init_chunk();
		chunks.push_back(c);
		chunks[index].mxneighbor = &chunks[chunks.size()-1];
		chunks[chunks.size()-1].mxneighbor = &chunks[index];
	}
	if (chunks[index].pxneighbor == NULL)
	{
		Chunk c;
		c.xoff = x+1;
		c.zoff = z;
		c.init_chunk();
		chunks.push_back(c);
		chunks[index].pxneighbor = &chunks[chunks.size()-1];
		chunks[chunks.size()-1].pxneighbor = &chunks[index];
	}
	if (chunks[index].mzneighbor == NULL)
	{
		Chunk c;
		c.xoff = x;
		c.zoff = z-1;
		c.init_chunk();
		chunks.push_back(c);
		chunks[index].mzneighbor = &chunks[chunks.size()-1];
		chunks[chunks.size()-1].mzneighbor = &chunks[index];
	}
	if (chunks[index].pzneighbor == NULL)
	{
		Chunk c;
		c.xoff = x;
		c.zoff = z+1;
		c.init_chunk();
		chunks.push_back(c);
		chunks[index].pzneighbor = &chunks[chunks.size()-1];
		chunks[chunks.size()-1].pzneighbor = &chunks[index];
		// bind_neighbors(chunks[index].pzneighbor.xoff, chunks[index].pzneighbor.zoff, )
	}

}