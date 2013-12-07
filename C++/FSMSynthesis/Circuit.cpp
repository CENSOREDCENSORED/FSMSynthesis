#include "Circuit.h"
#include <iostream>
#include <sstream>

#define DEFAULT_NUM_SCAN 10
#define DEFAULT_SIZE_SCAN 10

Circuit::Circuit()
{
	initCircuit(DEFAULT_NUM_SCAN, DEFAULT_SIZE_SCAN, false);
}

Circuit::Circuit(int numScan, int sizeScan, bool hasTrojan)
{
	initCircuit(numScan, sizeScan, hasTrojan);
}

void Circuit::initCircuit(int numScan, int sizeScan, bool hasTrojan)
{
	mySizeScan = sizeScan;
	myNumScan = numScan;

	myScanChainArr = new ScanChain * [myNumScan];
	for (int i = 0; i < myNumScan; i++)
	{
		myScanChainArr[i] = new ScanChain(mySizeScan, i);
	}
}

Circuit::~Circuit()
{
	for (int i = 0; i < myNumScan; i++)
	{
		delete myScanChainArr[i];
	}

	for (int i = 0; i < myNonSCWires.size(); i++)
	{
		delete myNonSCWires.at(i);
	}

	for (int i = 0; i < myGates.size(); i++)
	{
		delete myGates.at(i);
	}

	delete myScanChainArr;
}

int Circuit::SimulateStep()
{
	int numSwitches = 0;

	vector<Wire *> wireChangeQueue;
	for (int i = 0; i < myNumScan; i++)
	{
		for (int j = 0; j < mySizeScan; j++)
		{
			wireChangeQueue.push_back(myScanChainArr[i]->getFlopNet(j));
		}
	}

	for (int i = 0; i < wireChangeQueue.size(); i++)
	{
		Wire * wire = wireChangeQueue.at(i);
		for (int j = 0; j < myGates.size(); j++)
		{

		}
	}

	return numSwitches;
}

int Circuit::doPowerSimulation()
{
	int numSwitches = 0;

	//Initialization Step
	for (int i = 0; i < myNumScan; i++)
	{
		myScanChainArr[i]->seedScanChain(i+1);
	}

	//Allow values to reach combinational steady state.
	SimulateStep();

	//The actual simulation

	return numSwitches;
}

void Circuit::genRandomCircuit(int seed, unsigned int baseGates, 
							   unsigned int offsetGates, unsigned int offsetOutputs)
{
	//unsigned int baseGates = 1000;
	//unsigned int offsetGates = 1000;
	//unsigned int offsetOutputs = 100;
	unsigned int numiter = (rand() % offsetGates) + baseGates;

	srand(seed);

	for (int i = 0; i < myNumScan; i++)
	{
		for (int j = 0; j < mySizeScan; j++)
		{
			myWires.push_back(myScanChainArr[i]->getFlopNet(j));
			//myWires.at(i*mySizeScan + j)->setIsOutput(false);
		}
	}

	int numOutputs = myWires.size() + (rand() % offsetOutputs);

	for (int i = 0; i < numiter; i++)
	{
		int numWires = myWires.size();
		int input1 = rand() % numWires;
		int input2 = rand() % numWires;
		Wire * output = new Wire();
		output->setIsInput(false);
		NandGate * nandGate = new NandGate(myWires.at(input1), myWires.at(input2), output);
		
		string name = "Wire";
		ostringstream convert;
		convert << i;
		name.append(convert.str());
		output->setName(name);

		myWires.push_back(output);
		myNonSCWires.push_back(output);
		myGates.push_back(nandGate);
	}
}

void Circuit::printGates()
{
	for (int i = 0; i < myGates.size(); i++)
	{
		cout << "Gate #" << i << ":" << myGates.at(i)->getIn1()->getName() << "," << myGates.at(i)->getIn2()->getName() 
			<< "," << myGates.at(i)->getOut()->getName() << endl;
	}
}

void Circuit::genVerilog()
{

}
