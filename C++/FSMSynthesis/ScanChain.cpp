#include "ScanChain.h"
#include <iostream>

#define DEFAULT_SIZE 10
using namespace std;


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
	myFlopArr = new Wire *[mySize];
	for (int i = 0; i < mySize; i++)
	{
		myFlopArr[i] = new Wire();
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

void ScanChain::loadScanChain(int num)
{
	for (int i = 0; i < mySize; i++)
	{
		myFlopArr[i]->setVal(1 & (num >> i));
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


//Wire
Wire::Wire()
{
	myVal = 0;
}

Wire::~Wire()
{
}

void Wire::setVal(unsigned char newVal)
{
	myVal = newVal;
}

unsigned char Wire::getVal()
{
	return myVal;
}


//NandGate
NandGate::NandGate(Wire * in1, Wire * in2, Wire * out)
{
	myIn1 = in1;
	myIn2 = in2;
	myOut = out;
}

NandGate::~NandGate()
{
}

bool NandGate::genOutput()
{
	unsigned char origOut = myOut->getVal();
	myOut->setVal((!(myIn1->getVal() & myIn2->getVal())) & 1);
	return origOut != myOut->getVal();
}

Wire * NandGate::getOut()
{
	return myOut;
}

Wire * NandGate::getIn1()
{
	return myIn1;
}

Wire * NandGate::getIn2()
{
	return myIn2;
}
