#ifndef __TERRAIN_H
#define __TERRAIN_H

float noise(float x, float y);

class Terrain
{
public:
	int vertice_count;
	float scl;
	int rows, cols;
	glm::vec3 vertices[20000]; // rows * cols * 2
	Terrain(void);
	Terrain(int c, int r);
	void generate_coordinates(void);
	void generate_blocks(void);
private:
};

Terrain::Terrain(void)
{
	cols = 100;
	rows = 100;
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
	vector<glm::vec3> vert;
	scl = 3.0f;
	float terr[rows][cols];

	float yoff = 0.0f;
	for (int y = 0; y < rows; y++)
	{
		float xoff = 0.0f;
		for (int x = 0; x < cols; x++)
		{
			terr[x][y] = MAP(noise(xoff, yoff),-1,1,-8,8);
			xoff += 0.1f;
		}
		yoff += 0.1f;
	}

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols - 1/* idk.. (something is weird with terr and this+ 1 */; x++)
		{
			vert.push_back(glm::vec3(x * scl, y * scl, terr[x][y])); // even row
			vert.push_back(glm::vec3(x * scl, (y + 1.0f) * scl, terr[x][y+1])); // odd row
		}
	}
	vertice_count = vert.size();

	for (int i = 0; i < vertice_count; i++)
	{
		vertices[i] = vert[i];
	}
}

void Terrain::generate_blocks(void)
{

}

#endif