#include "ScanChain.h"


class Circuit
{
private:
	ScanChain ** myScanChainArr;
	int myNumScan, mySizeScan;
	vector<NandGate*> myGates;
	vector<Wire*> myWires;
	vector<Wire*> myNonSCWires;
	vector<Wire*> trojanWires;
	bool myHasTrojan;
	int trojanIteration;

	Wire * genWireNonInput();

public:
	Circuit();
	Circuit(int numScan, int sizeScan, bool hasTrojan);
	void initCircuit(int numScan, int sizeScan, bool hasTrojan);
	~Circuit();

	int SimulateStep();
	void seedScanChains();
	int genNextPowerMeasurement();
	int genAdder(int, int);
	int genComparator(int, int);
	int genMux(int, int, int);
	int addNANDGate(Wire *, Wire *, Wire *, int);
	int addANDGate(Wire *, Wire *, Wire *, int);
	int addXORGate(Wire *, Wire *, Wire *, int);
	void genRandomCircuit(int seed, unsigned int baseGates = 100, 
							   unsigned int offsetGates = 100, unsigned int offsetOutputs = 100);

	void printGates();
	void genVerilog();
};

