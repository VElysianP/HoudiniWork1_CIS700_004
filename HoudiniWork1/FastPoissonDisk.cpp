
#include "FastPoissonDisk.h"

//initialization, makes all of the data -1
FastPoissonDisk::FastPoissonDisk() : 
	radius(sqrt(DIMENSIONS_N) / MAX_SAMPLES), cellWidth(1 / MAX_SAMPLES), sampleAmount(0)
{
	for (int i = 0;i < MAX_SAMPLES;i++)
	{
		for (int j = 0;j < MAX_SAMPLES;j++)
		{
			for (int k = 0;k < MAX_SAMPLES;k++)
			{
				grid[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k] = -1;
				activeList[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k] = -1;
				samplePointsList[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k].x = -1.0f;
				samplePointsList[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k].y = -1.0f;
				samplePointsList[i*MAX_SAMPLES*MAX_SAMPLES + j*MAX_SAMPLES + k].z = -1.0f;
			}
		}
	}
}

//If the cell is empty, return -1. If it is not empty, it should return the index of the cell
//Also the index of the grid should be (k,j,i)
int FastPoissonDisk::SearchGridList(int i, int j, int k)
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
	Eigen::Vector3i gridLocationTestPoint = GridLocation(testPoint);

	int sampleTimeCount = 0;
	while (sampleTimeCount < MAX_TEST_K)
	{
		sampleTimeCount++;

		//the coordinates of the samplePoint is between 0.0 and 5*cellWidth
		Eigen::Vector3f samplePoint;
		samplePoint.x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*5.0f*cellWidth;
		samplePoint.y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*5.0f*cellWidth;
		samplePoint.z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*5.0f*cellWidth;

		//start sample from (i-2,j-2,k-2)

		Eigen::Vector3i samplePointGridLocation = GridLocation(samplePoint);

		samplePointGridLocation.x = samplePointGridLocation.x + gridLocationTestPoint.x - 2;
		samplePointGridLocation.y = samplePointGridLocation.y + gridLocationTestPoint.y - 2;
		samplePointGridLocation.z = samplePointGridLocation.z + gridLocationTestPoint.z - 2;

		//we do not have to check for the testPoint cell
		if (samplePointGridLocation==gridLocationTestPoint)
		{
			continue;
		}

		//if the grid here is empty, which means the number inside the grid is -1
		if (SearchGridList(samplePointGridLocation.z, samplePointGridLocation.y, samplePointGridLocation.x) == -1)
		{

		}
		//which means the cell is occupied
		else
		{
			continue;
		}

	}
	
	

}


//false means that the distance between the testPoint and the sample inside cell gridCoord is larger than radius
//true means that the distance between the testPoint and the sample inside cell gridCoor is smaller than radius
//which means that we should not add this point into sample list 
bool FastPoissonDisk::CheckWitinRadius(Eigen::Vector3f testPoint, Eigen::Vector3i gridCoord)
{
	int sampleIndex = SearchGridList(gridCoord.z, gridCoord.y, gridCoord.x);

	if (sampleIndex == -1)
	{
		return false;
	}
	else
	{
		Eigen::Vector3f thisSample = samplePointsList[sampleIndex];
		if((testPoint-thisSample))
	}
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
	sampleAmount++;
}

//sampleCount is the index of the sample that will be inserted inside the activeList
void FastPoissonDisk::InsertIntoActiveList(int sampleCount)
{
	if (sampleCount > (MAX_SAMPLES*MAX_SAMPLES*MAX_SAMPLES))
	{
		return;
	}
	int insertIndex = CheckActiveListSize();
	activeList[insertIndex] = sampleCount;
}

//this function will check how many samples indicies are there inside this active list
//and it will return the size of the active list
//which means the next index we can insert would be i
//if active list has nothing, it will return 0
int FastPoissonDisk::CheckActiveListSize()
{
	int i = 0;
	while (activeList[i] > -1)
	{
		i++;
	}
	return i;
}

//the grid location start from (0,0,0)
Eigen::Vector3i FastPoissonDisk::GridLocation(Eigen::Vector3f newSamplePoint)
{
	Eigen::Vector3i gridLocation = Eigen::Vector3i(floor(newSamplePoint.x / cellWidth), floor(newSamplePoint.y / cellWidth), floor(newSamplePoint.z / cellWidth));
	return gridLocation;
}

int FastPoissonDisk::GridArrayIndex(Eigen::Vector3i gridLocation)
{
	int girdArrayIndex = gridLocation.x + gridLocation.y*MAX_SAMPLES + gridLocation.z*MAX_SAMPLES*MAX_SAMPLES;
	return girdArrayIndex;
}

//the remove Index start from 0, it should be the index in the active list
void FastPoissonDisk::removeFromActiveList(int removeIndex)
{
	if (removeIndex > (MAX_SAMPLES*MAX_SAMPLES*MAX_SAMPLES))
	{
		return;
	}

	int i = removeIndex;
	int activeListSize = CheckActiveListSize();
	for (int i = removeIndex;i < activeListSize;i++)
	{
		activeList[i] = activeList[i + 1];
	}
}

void FastPoissonDisk::GenerationProcess()
{
	Eigen::Vector3f firstSample = GenerateOriginalSamplePoints();
	sampleAmount++;
	int sampleCount = sampleAmount - 1;

	//insert the first sample into three lists
	InsertIntoGridAndSampleList(firstSample, sampleCount);
	InsertIntoActiveList(sampleCount);

	int activeListSize = CheckActiveListSize();

	while (activeListSize !=0)
	{
		//randomIndex start from 0
		int randomIndex = (int)((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*(activeListSize-1));

	}


}

void FastPoissonDisk::PrintSampleLocations()
{
	for (int i = 0;i < sampleAmount;i++)
	{
		printf("The sample list we generated is:\n");
		printf("%15d", samplePointsList[i].x);
		printf("%15d", samplePointsList[i].y);
		printf("%15d", samplePointsList[i].z);
		printf("\n");
	}
}