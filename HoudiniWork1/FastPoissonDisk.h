#pragma once
#define MAX_SAMPLES 50
#define DIMENSIONS_N 3
#define MAX_TEST_K 30

using namespace std;

#include "stdafx.h"
#include "Eigen\Dense"
#include <vector>
#include <math.h>

class FastPoissonDisk{

public:
	FastPoissonDisk();

	int SearchActiveList(int i, int j, int k);

	Eigen::Vector3f GenerateOriginalSamplePoints();

	Eigen::Vector3f GenerateOtherSamples(Eigen::Vector3f testPoint);

	void InsertIntoGridAndSampleList(Eigen::Vector3f newSamplePoint, int sampleCount);

	void GenerationProcess();

	//use to test whether there is any sample inside this cell
	int grid[MAX_SAMPLES*MAX_SAMPLES*MAX_SAMPLES];

	//used to store the generate samples
	Eigen::Vector3f samplePointsList[MAX_SAMPLES*MAX_SAMPLES*MAX_SAMPLES];

	//since we will place the grid between (0,0,0) and (1,1,1), so r should be
	float radius = sqrt(3.0) / 50.0;

	//therefore the cell width would be
	float cellWidth = 0.02;

};






