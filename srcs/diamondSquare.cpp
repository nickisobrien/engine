#include "engine.h"

static void printArray(int Array[CHUNK_X][CHUNK_Z], int reach = 0)
{
	cout << reach << endl;
	for (int i = 0; i < CHUNK_X; i++)
	{
		for (int j = 0; j < CHUNK_Z; j++)
		{
			if (Array[i][j] < 10)
				cout << " ";
			cout << Array[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
}


static void diamondStep(int Array[CHUNK_X][CHUNK_Z], int x, int z, int reach)
{
	int count = 0;
	float avg = 0.0f;
	if (x - reach >= 0)
	{
		avg += Array[x-reach][z];
		count++;
	}
	if (x + reach < CHUNK_X)
	{
		avg += Array[x+reach][z];
		count++;
	}
	if (z - reach >= 0)
	{
		avg += Array[x][z-reach];
		count++;
	}
	if (z + reach < CHUNK_Z)
	{
		avg += Array[x][z+reach];
		count++;
	}
	avg /= count;
	Array[x][z] = (int)avg;
}

static void squareStep(int Array[CHUNK_X][CHUNK_Z], int x, int z, int reach)
{
	int count = 0;
	float avg = 0.0f;
	if (x - reach >= 0 && z - reach >= 0)
	{
		avg += Array[x-reach][z-reach];
		count++;
	}
	if (x - reach >= 0 && z + reach < CHUNK_Z)
	{
		avg += Array[x-reach][z+reach];
		count++;
	}
	if (x + reach < CHUNK_X && z - reach >= 0)
	{
		avg += Array[x+reach][z-reach];
		count++;
	}
	if (x + reach < CHUNK_X && z + reach < CHUNK_Z)
	{
		avg += Array[x+reach][z+reach];
		count++;
	}
	avg /= count;
	Array[x][z] = round(avg);
}

void diamondSquare(int Array[CHUNK_X][CHUNK_Z], int size)
{
	int half = size / 2;
	if (half < 1)
		return ;
	// cout << "size:" << size << " half: " << half << endl;

	// cout << "Square steps:" << endl;
	for (int z = half; z < CHUNK_Z; z+=size)
	{
		for (int x = half; x < CHUNK_X; x+=size)
		{
			// cout << x % CHUNK_X << " " << z % CHUNK_Z << " " << half << endl;
			squareStep(Array, x % CHUNK_X, z % CHUNK_Z, half);
			// printArray(Array);
		}
	}

	// cout << "Diamond steps:" << endl;
	int col = 0;
	for (int x = 0; x < CHUNK_X; x += half)
	{
		col++;
		//If this is an odd column.
		if (col % 2 == 1)
		{
			for (int z = half; z < CHUNK_Z; z += size)
			{
				// cout << "odd col" << x % CHUNK_X << " " << z % CHUNK_Z << " " << half << endl;
				diamondStep(Array, x % CHUNK_X, z % CHUNK_Z, half);
				// printArray(Array);
			}
		}
		else
		{
			for (int z = 0; z < CHUNK_Z; z += size)
			{
				// cout << "even col" << x % CHUNK_X << " " << z % CHUNK_Z << " " << half << endl;
				diamondStep(Array, x % CHUNK_X, z % CHUNK_Z, half);
				// printArray(Array);
			}
		}
	}

	// cout << "New loop" <<  endl;
	diamondSquare(Array, size / 2);
}