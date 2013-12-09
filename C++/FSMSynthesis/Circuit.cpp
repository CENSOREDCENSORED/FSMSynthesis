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

void Circuit::genAdder(int width, int baseWireIndex)
{

}

void Circuit::genComparator(int width, int baseWireIndex)
{
	if (myWires.size() == 0)
	{
		for (int i = 0; i < myNumScan; i++)
		{
			for (int j = 0; j < mySizeScan; j++)
			{
				myWires.push_back(myScanChainArr[i]->getFlopNet(j));
				//myWires.at(i*mySizeScan + j)->setIsOutput(false);
			}
		}
	}

	int bwi = baseWireIndex;
	vector<Wire *> compOutputs;
	//Only want to choose from existing wires, not ones added
	int numWires = myWires.size();
	for (int i = 0; i < width; i++)
	{
		int input1 = rand() % numWires;
		int input2 = rand() % numWires;

		Wire * in1 = myWires.at(input1);
		Wire * in2 = myWires.at(input2);

		Wire * outputIn1Inv = new Wire();
		outputIn1Inv->setIsInput(false);
		
		Wire * outputIn2Inv = new Wire();
		outputIn2Inv->setIsInput(false);
		
		Wire * outputNand1 = new Wire();
		outputNand1->setIsInput(false);
		
		Wire * outputNand2 = new Wire();
		outputNand2->setIsInput(false);
		
		Wire * outputNand3 = new Wire();
		outputNand3->setIsInput(false);


		addGate(in1,in1,outputIn1Inv,bwi++);
		addGate(in2,in2,outputIn2Inv,bwi++);
		addGate(in1, outputIn2Inv, outputNand1, bwi++);
		addGate(outputIn1Inv, in2, outputNand2, bwi++);
		addGate(outputNand1, outputNand2, outputNand3, bwi++);

		compOutputs.push_back(outputNand3);
	}


}

void Circuit::genMux(int width, int numSelects, int baseWireIndex)
{

}

void Circuit::addGate(Wire * in1, Wire * in2, Wire * out, int baseWireIndex)
{
	NandGate * nandGate = new NandGate(in1, in2, out);

	string name = "Wire";
	ostringstream convert;
	convert << baseWireIndex;
	name.append(convert.str());
	out->setName(name);

	myWires.push_back(out);
	myNonSCWires.push_back(out);
	myGates.push_back(nandGate);

}

void Circuit::genRandomCircuit(int seed, unsigned int baseGates, 
							   unsigned int offsetGates, unsigned int offsetOutputs)
{
	srand(seed);

	unsigned int numiter = (rand() % offsetGates) + baseGates;

	if (myWires.size() == 0)
	{
		for (int i = 0; i < myNumScan; i++)
		{
			for (int j = 0; j < mySizeScan; j++)
			{
				myWires.push_back(myScanChainArr[i]->getFlopNet(j));
				//myWires.at(i*mySizeScan + j)->setIsOutput(false);
			}
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

		addGate(myWires.at(input1), myWires.at(input2), output, i);
		
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
