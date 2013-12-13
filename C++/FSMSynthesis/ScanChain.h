#include "FSMSimulator.h"
#include "DigitalLogic.h"



class ScanChain
{
private:
	int mySize;
	Wire ** myFlopArr;
	RandNumGenerator * myRandNumGenerator;

	int loadScanChain(int);

public:
	ScanChain();
	ScanChain(int size, int scanChainNum);
	~ScanChain();

	void initScanChain(int scanChainNum);
	int seedScanChain(unsigned long long);
	int incrementScanChain();
	void printScanChain();

	int getSize();
	unsigned char getFlopVal(int);
	unsigned long long getCurrNum();
	Wire * getFlopNet(int);
};