#ifndef __TERRAIN_H
#define __TERRAIN_H

float noise(float x, float y);

class Terrain
{
public:
	int vertice_count;
	int scl;
	int rows, cols;
	glm::vec3 vertices[100000]; // rows * cols * 2
	Terrain();
private:
	float *generate_coordinates(const int vc);
};

Terrain::Terrain()
{
	cols = 100;
	rows = 100;
	vector<glm::vec3> vert;
	float scl = 5.0f;
	float terr[rows][cols];

	float yoff = 0.0f;
	for (int y = 0; y < rows; y++)
	{
		float xoff = 0.0f;
		for (int x = 0; x < cols; x++)
		{
			terr[x][y] = MAP(noise(xoff, yoff),-1,1,-10,3);
			// terr[x][y] = perlin(xoff, yoff);
			xoff += 0.1f;
		}
		yoff += 0.1f;
	}

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols - 1/* idfk (something is weird with terr and this+ 1 */; x++)
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

#endif