#include "ScanChain.h"

#define DEFAULT_SIZE 10

ScanChain::ScanChain(int size)
{
	mySize = size; 
	initScanChain();
}

ScanChain:: ScanChain()
{
	mySize = DEFAULT_SIZE; 
	initScanChain();
}

void ScanChain::initScanChain()
{
	myFlopArr = new unsigned char[mySize];
	myRandNumGenerator = new RandNumGenerator();
	myRandNumGenerator->setPolynomial(myRandNumGenerator->getPolynomial(mySize));
}

ScanChain::~ScanChain()
{
	delete myFlopArr;
	delete myRandNumGenerator;
}

void ScanChain::loadScanChain(int num)
{
	for (int i = 0; i < mySize; i++)
	{
		myFlopArr[i] = 1 & (num >> i);
	}
}

void ScanChain::initScanChain(unsigned long long seed)
{
	myRandNumGenerator->seedRandNumGen(seed);
	loadScanChain(seed);
}

void ScanChain::incrementScanChain()
{
	loadScanChain(myRandNumGenerator->genRandNum());
}