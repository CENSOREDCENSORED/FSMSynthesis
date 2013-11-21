#include "ErrorDetectionNetwork.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>


ErrorDetectionNetwork::ErrorDetectionNetwork()
{
	ErrorDetectionNetwork(None);
}

ErrorDetectionNetwork::ErrorDetectionNetwork(ErrorDetectionNetworkType type)
{
	myEDNT = type;
	ec = new ErrorCorrection();

	irreduciblePolynomials[0] = 0x1;
	irreduciblePolynomials[1] = 0x3;
	irreduciblePolynomials[2] = 0x7;
	irreduciblePolynomials[3] = 0xD;
	irreduciblePolynomials[4] = 0x19;
	irreduciblePolynomials[5] = 0x29;
	irreduciblePolynomials[6] = 0x61;
	irreduciblePolynomials[7] = 0xC1;
	irreduciblePolynomials[8] = 0x171;
	irreduciblePolynomials[9] = 0x221;
	irreduciblePolynomials[10] = 0x481;

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
		return ec->genSingleBitParity(data);
	case Hamming2and1:
		return 0;
	case Linear:
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
		return 1;
	case Hamming2and1:
		return 0;
	case Linear:
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
		return ec->genSingleBitParity(data) != check;
	case Hamming2and1:
		return false;
	case Linear:
		return ec->genHammingCodeParity(data) != check;
	case Nonlinear:
		return ec->nonLinearize(ec->GFMult(data, randNumber,irrPoly,numElems),irrPoly,numElems) != check;
	case Multilinear:
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

void ErrorDetectionNetwork::setRandNumGenPolynomial(unsigned int polynomial)
{
	rng->setPolynomial(polynomial);
}

unsigned int ErrorDetectionNetwork::getPolynomial(int degree)
{
	return irreduciblePolynomials[degree];
}

unsigned int ErrorDetectionNetwork::getRandNumber()
{
	return rng->genRandNum();
}

void ErrorDetectionNetwork::seedRNG(unsigned int seed)
{
	rng->seedRandNumGen(seed);
}