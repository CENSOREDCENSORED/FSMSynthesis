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
	void seedScanChains(unsigned long long * seeds = 0, int numSeeds = 0, int numAdvances = 0);
	int genNextPowerMeasurement(int scanChainIncrementIndex = -1, bool partition = false,
		int numPartitionGroups = 0, int numPartitions = 0, int partitionSize = 0, int currPartitionGroup = 0,
		bool printPartitionGroups = false,
		int * scanChainIncrementVals = nullptr);
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

	void printNumGates();

	unsigned long long * getScanChainVals();
	int getNumScanChains();
};

