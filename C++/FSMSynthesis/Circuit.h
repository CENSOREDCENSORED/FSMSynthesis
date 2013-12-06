#include "ScanChain.h"


class Circuit
{
private:
	ScanChain ** myScanChainArr;
	int myNumScan, mySizeScan;
	vector<NandGate*> myGates;
	vector<Wire*> myWires;
	vector<Wire*> myNonSCWires;

public:
	Circuit();
	Circuit(int numScan, int sizeScan, bool hasTrojan);
	void initCircuit(int numScan, int sizeScan, bool hasTrojan);
	~Circuit();

	int SimulateOneTest(bool);
	void genRandomCircuit(int);

	void printGates();
	void genVerilog();
};

