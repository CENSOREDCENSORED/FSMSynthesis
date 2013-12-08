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
	myHasTrojan = hasTrojan;

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

	bool iterateAgain = true;

	while (iterateAgain)
	{
		iterateAgain = false;
		for (int i = 0; i < myGates.size(); i++)
		{
			NandGate * gate = myGates[i];
			Wire * out = gate->getOut();

			unsigned char oldOutputVal = out->getVal();

			gate->genOutput();

			if (out->getNewVal() != oldOutputVal)
			{
				iterateAgain = true;
				//cout << out->getName() << endl;
				numSwitches++;
			}
		}

		for (int i = 0; i < myGates.size(); i++)
		{
			myGates[i]->getOut()->advanceVal();
		}
	}

	return numSwitches;
}

void Circuit::seedScanChains()
{
	//Initialization Step
	for (int i = 0; i < myNumScan; i++)
	{
		myScanChainArr[i]->seedScanChain(i+1);
	}

	//Allow values to reach combinational steady state.
	//cout << SimulateStep() << endl;
	SimulateStep();
}

int Circuit::genNextPowerMeasurement()
{
	int numSwitches = 0;

	for (int i = 0; i < myNumScan; i++)
	{
		numSwitches += myScanChainArr[i]->incrementScanChain();
	}
	numSwitches += SimulateStep();

	if (myHasTrojan)
	{
		trojanWires.at(trojanIteration);
	}

	return numSwitches;
}

void Circuit::genRandomCircuit(int seed, unsigned int baseGates, 
							   unsigned int offsetGates, unsigned int offsetOutputs)
{
	srand(seed);

	unsigned int numiter = (rand() % offsetGates) + baseGates;

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

	if (myHasTrojan)
	{
		//Put in trojan circuitry here
		//assume between 10-19 steps to activate trojan
		
		trojanIteration = 0;
		int numTrojanIterations = (rand() % 10) + 10;

		for (int i = 0; i < numTrojanIterations; i++)
		{
			int numDependentWires = rand() % 4;
			
			//TODO: insert new gates maybe???
			int wireIndex = rand() % myWires.size();
			
			trojanWires.push_back(myWires.at(wireIndex));
		}

		/*
		int numTrojanGates = rand() % 10 + 10;
		for (int i = numiter; i < numiter+numTrojanGates; i++)
		{
			
		}
		*/
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
