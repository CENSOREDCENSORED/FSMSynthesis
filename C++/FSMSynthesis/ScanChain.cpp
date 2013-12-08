#include "ScanChain.h"
#include <iostream>
#include <sstream>

#define DEFAULT_SIZE 10
using namespace std;


ScanChain::ScanChain(int size, int scanChainNum)
{
	mySize = size; 
	initScanChain(scanChainNum);
}

ScanChain:: ScanChain()
{
	mySize = DEFAULT_SIZE; 
	initScanChain(0);
}

void ScanChain::initScanChain(int scanChainNum)
{
	myFlopArr = new Wire *[mySize];
	for (int i = 0; i < mySize; i++)
	{
		myFlopArr[i] = new Wire();
		string name = "ScanChain";
		ostringstream convert;
		convert << scanChainNum;
		name.append(convert.str());
		convert.str("");
		convert.clear();
		name.append("-");
		convert << i;

		name.append(convert.str());

		myFlopArr[i]->setName(name);
	}
	myRandNumGenerator = new RandNumGenerator();
	myRandNumGenerator->setPolynomial(myRandNumGenerator->getPolynomial(mySize) >> 1);
}

ScanChain::~ScanChain()
{
	for (int i = 0; i < mySize; i++)
	{
		delete myFlopArr[i];
	}
	delete myFlopArr;
	delete myRandNumGenerator;
}

int ScanChain::loadScanChain(int num)
{
	int numSwitches = 0;
	for (int i = 0; i < mySize; i++)
	{
		unsigned char oldVal = myFlopArr[i]->getVal();
		unsigned char newVal = 1 & (num >> i);
		if (oldVal != newVal) numSwitches++;
		myFlopArr[i]->setVal(newVal);
		myFlopArr[i]->advanceVal();
	}
	return numSwitches;
}

int ScanChain::seedScanChain(unsigned long long seed)
{
	myRandNumGenerator->seedRandNumGen(seed);
	return loadScanChain(seed);
}

int ScanChain::incrementScanChain()
{
	return loadScanChain(myRandNumGenerator->genRandNum());
}

void ScanChain::printScanChain()
{
	for (int i = 0; i < mySize; i++)
	{
		cout << (unsigned int) getFlopVal(i);
	} 
}

int ScanChain::getSize()
{
	return mySize;
}

unsigned char ScanChain::getFlopVal(int index)
{
	return myFlopArr[index]->getVal();
}

Wire * ScanChain::getFlopNet(int index)
{
	return myFlopArr[index];
}

