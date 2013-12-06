#include "FSMSimulator.h"
#include "DigitalLogic.h"



class ScanChain
{
private:
	int mySize;
	Wire ** myFlopArr;
	RandNumGenerator * myRandNumGenerator;

	void loadScanChain(int);

public:
	ScanChain();
	ScanChain(int size, int scanChainNum);
	~ScanChain();

	void initScanChain(int scanChainNum);
	void seedScanChain(unsigned long long);
	void incrementScanChain();
	void printScanChain();

	int getSize();
	unsigned char getFlopVal(int);
	Wire * getFlopNet(int);
};