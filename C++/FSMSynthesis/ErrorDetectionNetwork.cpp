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
	irreduciblePolynomials[3] = 0xB;
	irreduciblePolynomials[4] = 0x13;
	irreduciblePolynomials[5] = 0x25;
	//irreduciblePolynomials[6] = 
}

ErrorDetectionNetwork::~ErrorDetectionNetwork()
{
	delete ec;
}

int ErrorDetectionNetwork::genPrediction(int data, int randNumber)
{
	switch (myEDNT)
	{
	case None: 
		return 0;
	case Hamming2: 
		return 0;
	case Hamming2and1:
		return 0;
	case Linear:
		return ec->genHammingCodeParity(data);
	case Nonlinear:
		//return ec->nonLinearize(ec->GFMult(data, randNumber),,);
		return 0;
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
	case Linear:
		return ec->getHammingPredictionSize(dataSize);
	case Nonlinear:
		//ec->genNonLinearHammingCodeParity(data,,);
		return 0;
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
bool ErrorDetectionNetwork::doErrorCheck(int data, int check)
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
	case Linear:
		if (ec->genHammingCodeParity(data) != check) return true;
		return false;
	case Nonlinear:
		return false;
	case Multilinear:
		return false;
	}
}

void ErrorDetectionNetwork::setType(ErrorDetectionNetworkType ednt)
{
	myEDNT = ednt;
}