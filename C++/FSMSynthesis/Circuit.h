#include "ScanChain.h"

class Circuit
{
private:
	ScanChain ** myScanChainArr;
	int myNumScan, mySizeScan;

public:
	Circuit();
	Circuit(int numScan, int sizeScan, bool hasTrojan);
	~Circuit();

	int getPowerMeasurement();
};