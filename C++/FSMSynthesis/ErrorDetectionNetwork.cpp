#include "ErrorDetectionNetwork.h"
#include "ErrorCorrection.h"
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
}

ErrorDetectionNetwork::~ErrorDetectionNetwork()
{

}

int ErrorDetectionNetwork::genPrediction(int data)
{

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
	case Hamming2and1:
	case Linear:
	case Nonlinear:
	case Multilinear:
	}
}