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

	int numStates = 10;
	int numInputs = 4;
	int numOutputs = 4;
	int initialState = 3;

	int numTransitions = 100;

	//booleans
	bool testRandNumGen = false;
	bool testFSM = false;
	bool testPowAnalysis = true;
	bool seeOutput = true;

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

	if (testFSM)
	{
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
		FSMSim->simulateFSM(inputs, true, "ErrorCountNonLinearHamming.csv");

		EDN->setType(LinearHamming);

		FSMSim->simulateFSM(inputs, true, "ErrorCountLinear.csv");

		EDN->setType(LinearParity);

		FSMSim->simulateFSM(inputs, true, "ErrorCountLinearParity.csv");

		EDN->setType(Hamming2);
		FSM->setStateNames(Hamming2Encoding);
		FSMSim->simulateFSM(inputs, true, "ErrorCountHamming2.csv");


		delete FSMSim;
		delete EDN;
		delete FSM;
	}

	if (testPowAnalysis)
	{
		int trojanLength = 10;
		int sideChannelPowerOffset = 10;
		int * trojanSeq = new int[trojanLength];
		int * trojanIndex = new int[trojanLength];
		for (int i = 0; i < trojanLength; i++)
		{
			trojanSeq[i] = rand() % 2;
			trojanIndex[i] = rand() % trojanLength;
			cout << trojanSeq[i] << "," << trojanIndex[i] << endl;
		}
		RandNumGenerator * rng = new RandNumGenerator(0x481 >> 1);
		rng->seedRandNumGen(1);
		int count = 0;
		for (int i = 0; i < 100; i++)
		{
			//Pseudorandom normal distrubtion
			int powMeasurement = 0; 
			for (int i = 0; i < 16; i++)
			{
				powMeasurement += (rand() % 101) - 50;
			}

			powMeasurement /= 16;

			//input test vector via lfsr
			int var = rng->genRandNum();
			bool trojIndex = false; 
			if ((var >> trojanIndex[count]) & 1 == trojanSeq[count])
			{
				count++;
				powMeasurement += sideChannelPowerOffset;
				trojIndex = true;
			}
			else 
			{
				count = 0;
			}

			cout << var << "," << powMeasurement << "," << trojIndex << endl;
		}

		delete trojanSeq;
		delete trojanIndex;
	}

	if (seeOutput) while (true){}
}