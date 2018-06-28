#ifndef __TERRAIN_H
#define __TERRAIN_H

using namespace std;

class Terrain
{
public:
	int vertice_count;
	int scl;
	int rows, cols;
	float vertices[10000];
	Terrain();
private:
	float *generate_coordinates(const int vc);
};

Terrain::Terrain()
{
	cols = 50;
	rows = 50;

	int N = max(rows, cols) / 10;

	// vertices:
	vector<glm::vec3> vert;
	
	rows = 2;
	for (float i = -1; i < (float)rows - 1.0f; i++)
	{
		for (std::uint32_t j=0; j < N + 1; ++j)
		{
			float x = (float)j/(float)N;
			vert.push_back(glm::vec3(x - 0.5f, i, 0)); // even row
			vert.push_back(glm::vec3(x - 0.5f, i + 0.5, 0)); // odd row
		}
	}

	vertice_count = vert.size();
	// copying
	for (int i = 0; i < vertice_count * 3; i+=3)
	{
		vertices[i] = vert[i / 3].x;
		vertices[i+1] = vert[i / 3].y;
		vertices[i+2] = vert[i / 3].z;
	}
}

// float *Terrain::generate_coordinates(const int vc)
// {
// 	float *v;
// 	return (v);
// }

#endif