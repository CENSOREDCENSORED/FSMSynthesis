#include "ScanChain.h"


class Circuit
{
private:
	ScanChain ** myScanChainArr;
	int myNumScan, mySizeScan;
	vector<NandGate*> myGates;
	vector<Wire*> myWires;

public:
	Circuit();
	Circuit(int numScan, int sizeScan, bool hasTrojan);
	~Circuit();

	int getPowerMeasurement();
	void genRandomCircuit(int);
};

