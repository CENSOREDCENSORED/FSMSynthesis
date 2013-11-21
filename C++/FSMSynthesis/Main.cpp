#include "FSMSimulator.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

void main()
{
	int seed = 100;
	srand (seed);

	int numStates = 40;
	int numInputs = 4;
	int numOutputs = 4;
	int initialState = 3;

	int numTransitions = 100;

	//booleans
	bool testRandNumGen = false;
	bool seeOutput = false;

	if (testRandNumGen)
	{
		RandNumGenerator * rng = new RandNumGenerator(0x19 >> 1);
		rng->seedRandNumGen(1);
		int var = 1;
		for (int i = 0; i < 100; i++)
		{
			cout << var << endl;
			var = rng->genRandNum();
		}
	}

	//ErrorCorrection * ec = new ErrorCorrection();
	//cout << ec->genHammingCodeParity(1) << endl;

	FiniteStateMachine * FSM = new FiniteStateMachine(numStates, numInputs, numOutputs, initialState);
	//FSM->setOutputLogic(true);
	FSM->genRandomFSM();
	//FSM->printFSM();
	FSM->minimizeFSM();
	//FSM->printFSM();
	//FSM->genVerilog("../../Verilog/FSM.v");

	//ErrorDetectionNetwork * EDN = new ErrorDetectionNetwork(Hamming2);
	//ErrorDetectionNetwork * EDN = new ErrorDetectionNetwork(Linear);
	ErrorDetectionNetwork * EDN = new ErrorDetectionNetwork(Nonlinear);
	FSMSimulator * FSMSim = new FSMSimulator(FSM, EDN);

	vector<int> inputs;
	for (int i = 0; i < numTransitions; i++)
	{
		inputs.push_back(rand() % numInputs);
	}
	FSMSim->simulateFSM(inputs, true, "ErrorCountNonLinear.csv");

	EDN->setType(Linear);

	FSMSim->simulateFSM(inputs, true, "ErrorCountLinear.csv");

	EDN->setType(Hamming2);

	FSMSim->simulateFSM(inputs, true, "ErrorCountHamming2.csv");

	delete FSMSim;
	delete EDN;
	delete FSM;
	if (seeOutput) while (true){}
}