#include <fstream>
#include "Heightmap.h"

Heightmap::Heightmap()
{

}
	
Heightmap::~Heightmap()
{
	mHeightmap.clear();
}

void Heightmap::loadRAW(int rows, int colums, string filename, float heightScale, float heightOffset)
{
	// Set values.
	mHeightScale	= heightScale;
	mHeightOffset	= heightOffset;
	mRows	= rows;
	mColums = colums;

	// Resize the vector.
	mHeightmap.resize(rows*colums);

	// A height for each vertex
	std::vector<unsigned char> in(rows*colums);

	// Open the file.
	std::ifstream inFile;
	inFile.open(filename.c_str(), ios_base::binary);

	// Read the RAW bytes.
	inFile.read((char*)&in[0], (streamsize)in.size());

	for(int i = 0; i < mHeightmap.size(); i++)
		mHeightmap[i] = (float)in[i] * mHeightScale + mHeightOffset;

	// Done with file.
	inFile.close();

	filter3x3();
}

void Heightmap::filter3x3()
{
	vector<float> temp(mRows*mColums);

	for(int i = 0; i < mRows; ++i)
		for(int j = 0; j < mColums; ++j)
			temp[i*mColums + j] = sampleHeight3x3(i,j);

	mHeightmap = temp;
}

float Heightmap::sampleHeight3x3(int i, int j)
{
	float avg = 0.0f;
	float num = 0.0f;

	for(int m = i-1; m <= i+1; ++m)
	{
		for(int n = j-1; n <= j+1; ++n)
		{
			if( inBounds(m,n) ) {
				avg += mHeightmap[m*mColums + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

void Heightmap::addHeight(int i, int j, float height)
{
	mHeightmap[i*mColums + j] += height;
	filter3x3();
}

float& Heightmap::operator()(int i, int j)
{
	return (mColums*i + j) >= mHeightmap.size() ? mHeightmap[0] : mHeightmap[mColums*i + j];
}
	
bool Heightmap::inBounds(int i, int j)
{
	return i >= 0 && i < mRows && j >= 0 && j < mColums;
}

int Heightmap::numColums()
{
	return mColums;
}
	
int Heightmap::numRows()
{
	return mRows;
}