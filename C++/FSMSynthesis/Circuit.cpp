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

Wire * Circuit::genWireNonInput()
{
	Wire * newWire = new Wire();
	newWire->setIsInput(false);

	return newWire;
}

int Circuit::genAdder(int width, int baseWireIndex)
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

	//Only want to choose from existing wires, not ones added
	int numWires = myWires.size();
	vector<Wire *> carryOuts;

	for (int i = 0; i < width; i++)
	{
		int input1 = rand() % numWires;
		int input2 = rand() % numWires;

		Wire * in1 = myWires.at(input1);
		Wire * in2 = myWires.at(input2);
		Wire * carryIn;
		if (i == 0)
		{
			int input3 = rand() % numWires;
			carryIn = myWires.at(input3);
		}
		else
		{
			carryIn = carryOuts.at(carryOuts.size()-1);
		}

		//Generate the sum
		Wire * in1XORin2 = genWireNonInput();

		bwi = addXORGate(in1, in2, in1XORin2, bwi);

		Wire * sumOutput = genWireNonInput();

		bwi = addXORGate(in1XORin2, carryIn, sumOutput, bwi);

		//Generate the carry
		Wire * nandOutput1 = genWireNonInput();
		Wire * nandOutput2 = genWireNonInput();
		Wire * nandOutput3 = genWireNonInput();

		bwi = addNANDGate(in1, in2, nandOutput1, bwi);
		bwi = addNANDGate(in1, carryIn, nandOutput2, bwi);
		bwi = addNANDGate(carryIn, in2, nandOutput3, bwi);

		Wire * carryOut = genWireNonInput();
		
		Wire * AndOutput1 = genWireNonInput();
		bwi = addANDGate(nandOutput1, nandOutput2, AndOutput1, bwi);
		bwi = addANDGate(AndOutput1, nandOutput3, carryOut, bwi);

		carryOuts.push_back(carryOut);

	}

	return bwi;
}

int Circuit::genComparator(int width, int baseWireIndex)
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
	vector<Wire *> * compOutputs = new vector<Wire *>();
	//Only want to choose from existing wires, not ones added
	int numWires = myWires.size();
	for (int i = 0; i < width; i++)
	{
		int input1 = rand() % numWires;
		int input2 = rand() % numWires;

		Wire * in1 = myWires.at(input1);
		Wire * in2 = myWires.at(input2);
		Wire * in1XORin2 = genWireNonInput();
		bwi = addXORGate(in1, in2, in1XORin2, bwi);

		compOutputs->push_back(in1XORin2);
	}

	//TODO: Implement the final big AND gate
	if (compOutputs->size() > 1)
	{
		vector<Wire *> * newCompOutputs = new vector<Wire *>;
		for (int i = 0; i < compOutputs->size(); i = i + 2)
		{
			if ((i+1) == compOutputs->size())
			{
				newCompOutputs->push_back(compOutputs->at(i));
			}

			Wire * newOutput = genWireNonInput();

			bwi = addANDGate(compOutputs->at(i), compOutputs->at(i+1), newOutput, bwi);

			newCompOutputs->push_back(newOutput);
		}

		delete newCompOutputs;
	}

	delete compOutputs;

	return bwi;
}

int Circuit::genMux(int width, int numSelects, int baseWireIndex)
{
	int bwi = baseWireIndex;

	return bwi;
}

int Circuit::addANDGate(Wire * in1, Wire * in2, Wire * out, int baseWireIndex)
{
	int bwi = baseWireIndex;

	Wire * nandOut = genWireNonInput();

	addNANDGate(in1,in2,nandOut,bwi++);
	addNANDGate(nandOut,nandOut,out,bwi++);

	return bwi;

}

int Circuit::addXORGate(Wire * in1, Wire * in2, Wire * out, int baseWireIndex)
{
	int bwi = baseWireIndex;
	Wire * outputIn1Inv = genWireNonInput();
		
	Wire * outputIn2Inv = genWireNonInput();
		
	Wire * outputNand1 = genWireNonInput();
		
	Wire * outputNand2 = genWireNonInput();


	bwi = addNANDGate(in1,in1,outputIn1Inv,bwi);
	bwi = addNANDGate(in2,in2,outputIn2Inv,bwi);
	bwi = addNANDGate(in1, outputIn2Inv, outputNand1, bwi);
	bwi = addNANDGate(outputIn1Inv, in2, outputNand2, bwi);
	bwi = addNANDGate(outputNand1, outputNand2, out, bwi);

	return bwi;
}

int Circuit::addNANDGate(Wire * in1, Wire * in2, Wire * out, int baseWireIndex)
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

	return baseWireIndex + 1;
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
		Wire * output = genWireNonInput();

		addNANDGate(myWires.at(input1), myWires.at(input2), output, i);
		
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
