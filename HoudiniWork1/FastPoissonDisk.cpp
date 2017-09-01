
#include "FastPoissonDisk.h"

FastPoissonDisk::FastPoissonDisk()
{
	for (int i = 0;i < MAX_SAMPLES;i++)
	{
		for (int j = 0;j < MAX_SAMPLES;j++)
		{
			for (int k = 0;k < MAX_SAMPLES;k++)
			{
				grid[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k] = -1;
				samplePointsList[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k].x = -1;
				samplePointsList[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k].y = -1;
				samplePointsList[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k].z = -1;
			}
		}
	}
}

//If the cell is empty, return true. If it is not empty, it should return the index of the cell
//Also the index of the grid should be (k,j,i)
int FastPoissonDisk::SearchActiveList(int i, int j, int k)
{
	//if the grid is empty then the number inside should be -1
	int gridIndex = i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k;
	if (grid[gridIndex] == -1)
	{
		return -1;
	}
	else
	{
		return grid[gridIndex];
	}
}

//Make sure that we generate x0 within the range from 0.0 to 1.0
Eigen::Vector3f FastPoissonDisk::GenerateOriginalSamplePoints()
{
	Eigen::Vector3f samplePoint;
	samplePoint.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	samplePoint.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	samplePoint.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return samplePoint;
}


//radius between r and 2r should test the grid between (i-2,j-2,k-2) to (i+2,j+2,k+2)
Eigen::Vector3f FastPoissonDisk::GenerateOtherSamples(Eigen::Vector3f testPoint)
{
	Eigen::Vector3i gridLocation = GridLocation(testPoint);

}

//sampleCount here starts at 0
void FastPoissonDisk::InsertIntoGridAndSampleList(Eigen::Vector3f newSamplePoint,int sampleCount)
{
	if (sampleCount <= -1)
	{
		return;
	}
	samplePointsList[sampleCount].x = newSamplePoint.x;
	samplePointsList[sampleCount].y = newSamplePoint.y;
	samplePointsList[sampleCount].z = newSamplePoint.z;

	Eigen::Vector3i gridLocation = GridLocation(newSamplePoint);
	
	grid[GridArrayIndex(gridLocation)] = sampleCount;
}

Eigen::Vector3i FastPoissonDisk::GridLocation(Eigen::Vector3f newSamplePoint)
{
	Eigen::Vector3i gridLocation = Eigen::Vector3i((int)(newSamplePoint.x / cellWidth), (int)(newSamplePoint.y / cellWidth), (int)(newSamplePoint.x / cellWidth));
	return gridLocation;
}

int FastPoissonDisk::GridArrayIndex(Eigen::Vector3i gridLocation)
{
	int girdArrayIndex = gridLocation.x + gridLocation.y*MAX_SAMPLES + gridLocation.z*MAX_SAMPLES*MAX_SAMPLES;
	return girdArrayIndex;
}

void FastPoissonDisk::GenerationProcess()
{
	int sampleCount = -1;
	Eigen::Vector3f firstSample = GenerateOriginalSamplePoints();
	sampleCount++;

	InsertIntoGridAndSampleList(firstSample, sampleCount);



}

void FastPoissonDisk::PrintSampleLocations()
{
	for (int i = 0;i < MAX_SAMPLES;i++)
	{
		for (int j = 0;j < MAX_SAMPLES;j++)
		{
			for (int k = 0;k < MAX_SAMPLES;k++)
			{
				printf("The sample list we generated is:\n");
				printf("%15d", samplePointsList[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k].x);
				printf("%15d", samplePointsList[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k].y);
				printf("%15d", samplePointsList[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k].z);
				printf("\n");
			}
		}
	}
}