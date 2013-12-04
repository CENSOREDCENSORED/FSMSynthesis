#include "FSMSimulator.h"

class ScanChain
{
private:
	int mySize;
	unsigned char * myFlopArr;
	RandNumGenerator * myRandNumGenerator;

	void loadScanChain(int);

public:
	ScanChain();
	ScanChain(int size);
	~ScanChain();

	void initScanChain();
	void initScanChain(unsigned long long);
	void incrementScanChain();
};