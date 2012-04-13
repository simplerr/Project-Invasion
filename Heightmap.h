#pragma once

#include <vector>
#include <string>
using namespace std;

class Heightmap
{
public:
	Heightmap();
	~Heightmap();

	void loadRAW(int rows, int colums, string filename, float heightScale, float heightOffset);

	void addHeight(int i, int j, float height);

	int numColums();
	int numRows();

	float& operator()(int i, int j);
private:
	bool  inBounds(int i, int j);
	float sampleHeight3x3(int i, int j);
	void  filter3x3();
private:
	vector<float> mHeightmap;
	float mHeightScale;
	float mHeightOffset;
	int mColums;
	int mRows;
};