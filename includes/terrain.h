#ifndef __TERRAIN_H
#define __TERRAIN_H

using namespace std;

class Terrain
{
public:
	int vertice_count;
	int scl;
	int rows, cols;
	glm::vec3 vertices[10000];
	Terrain();
private:
	float *generate_coordinates(const int vc);
};

Terrain::Terrain()
{
	cols = 25;
	rows = 25;
	vector<glm::vec3> vert;
	float scl = 0.4f;

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols /* + 1 */; x++)
		{
	        vert.push_back(glm::vec3(x * scl, y * scl, 0)); // even row
	        vert.push_back(glm::vec3(x * scl, (y + 1.0f) * scl, 0)); // odd row
		}
	}

	vertice_count = vert.size();
	for (int i = 0; i < vertice_count; i++)
	{
		vertices[i] = vert[i];
	}
}

// float *Terrain::generate_coordinates(const int vc)
// {
// 	float *v;
// 	return (v);
// }

#endif