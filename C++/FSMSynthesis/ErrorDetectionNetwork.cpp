#include "ErrorDetectionNetwork.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>


ErrorDetectionNetwork::ErrorDetectionNetwork()
{
	myEDNT = None;
	ec = new ErrorCorrection();

	rng = new RandNumGenerator();
}

ErrorDetectionNetwork::ErrorDetectionNetwork(ErrorDetectionNetworkType type)
{
	myEDNT = type;
	ec = new ErrorCorrection();

	rng = new RandNumGenerator();
}

ErrorDetectionNetwork::~ErrorDetectionNetwork()
{
	delete ec;
	delete rng;
}

int ErrorDetectionNetwork::genPrediction(int data, int randNumber, unsigned int irrPoly, unsigned int numElems)
{
	switch (myEDNT)
	{
	case None: 
		return 0;
	case Hamming2: 
		return 0;
	case Hamming2and1:
		return 0;
	case LinearParity:
		return ec->genSingleBitParity(data);
	case LinearHamming:
		return ec->genHammingCodeParity(data);
	case Nonlinear:
		return ec->nonLinearize(ec->GFMult(data, randNumber,irrPoly,numElems),irrPoly,numElems);
	case Multilinear:
		return 0;
	default: return 0;
	}
}

int ErrorDetectionNetwork::getPredictionSize(int dataSize)
{
	switch (myEDNT)
	{
	case None: 
		return 0;
	case Hamming2: 
		return 0;
	case Hamming2and1:
		return 0;
	case LinearParity:
		return 1;
	case LinearHamming:
		return ec->getHammingPredictionSize(dataSize);
	case Nonlinear:
		//ec->genNonLinearHammingCodeParity(data,,);
		return dataSize;
	case Multilinear:
		return 0;
	default: return 0;
	}
}

void ErrorDetectionNetwork::genVerilog(string filename)
{
	ofstream myfile;
	myfile.open(filename);


	myfile.close();
}

//False means no error. True means error.
bool ErrorDetectionNetwork::doErrorCheck(int data, int check, int randNumber, unsigned int irrPoly, unsigned int numElems)
{
	if (myEDNT == None) return false;
	switch(myEDNT)
	{
	case None: 
		return false;
	case Hamming2:
		return false;
	case Hamming2and1:
		return false;
	case LinearParity:
		return ec->genSingleBitParity(data) != check;
	case LinearHamming:
		return ec->genHammingCodeParity(data) != check;
	case Nonlinear:
		return ec->nonLinearize(ec->GFMult(data, randNumber,irrPoly,numElems),irrPoly,numElems) != check;
	case Multilinear:
		return false;
	default: 
		return false;
	}
}

bool ErrorDetectionNetwork::usesRandom()
{
	if (myEDNT == Nonlinear || myEDNT == Multilinear) return true;
	return false; 
}

void ErrorDetectionNetwork::setType(ErrorDetectionNetworkType ednt)
{
	myEDNT = ednt;
}

void ErrorDetectionNetwork::setRandNumGenPolynomial(unsigned long long polynomial)
{
	rng->setPolynomial(polynomial);
}

unsigned long long ErrorDetectionNetwork::getPolynomial(int degree)
{
	return rng->getPolynomial(degree); 
}

unsigned long long ErrorDetectionNetwork::getRandNumber()
{
	return rng->genRandNum();
}

void ErrorDetectionNetwork::seedRNG(unsigned long long seed)
{
	rng->seedRandNumGen(seed);
}