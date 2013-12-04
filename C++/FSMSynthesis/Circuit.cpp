#include "Circuit.h"

#define DEFAULT_NUM_SCAN 10
#define DEFAULT_SIZE_SCAN 10

Circuit::Circuit()
{
	Circuit(DEFAULT_NUM_SCAN, DEFAULT_SIZE_SCAN, false);
}

Circuit::Circuit(int numScan, int sizeScan, bool hasTrojan)
{
	mySizeScan = sizeScan;
	myNumScan = numScan;

	myScanChainArr = new ScanChain * [myNumScan];
	for (int i = 0; i < myNumScan; i++)
	{
		myScanChainArr[i] = new ScanChain(mySizeScan);
	}
}

Circuit::~Circuit()
{
	for (int i = 0; i < myNumScan; i++)
	{
		delete myScanChainArr[i];
	}

	delete myScanChainArr;
}

int Circuit::getPowerMeasurement()
{
	return 1;
}