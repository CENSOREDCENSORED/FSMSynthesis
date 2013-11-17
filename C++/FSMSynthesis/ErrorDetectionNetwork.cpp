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
}

ErrorDetectionNetwork::~ErrorDetectionNetwork()
{
	delete ec;
}

int ErrorDetectionNetwork::genPrediction(int data)
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
		//ec->genNonLinearHammingCodeParity(data,,);
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

bool ErrorDetectionNetwork::doErrorCheck(int data, int check)
{
	if (myEDNT == None) return true;
	switch(myEDNT)
	{
	case None: 
		return true;
	case Hamming2: 
		return true;
	case Hamming2and1:
		return true;
	case Linear:
		if (ec->genHammingCodeParity(data) == check) return true;
		return false;
	case Nonlinear:
		return true;
	case Multilinear:
		return true;
	}
}