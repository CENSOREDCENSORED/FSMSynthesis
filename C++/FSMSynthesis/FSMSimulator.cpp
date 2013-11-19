#include "FSMSimulator.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

FSMSimulator::FSMSimulator()
{
	FSMSimulator(nullptr, nullptr);
}

FSMSimulator::FSMSimulator(FiniteStateMachine * fsm, ErrorDetectionNetwork * edn)
{
	myEDN = edn;
	myFSM = fsm;
}

FSMSimulator::~FSMSimulator()
{

}

void FSMSimulator::simulateFSM(vector<int> inputs, bool doErrorAnalysis)
{
	bool showStates = false;

	int index = 0;
	int data = myFSM->getNumStates();
	int tempData = data;
	while (tempData != 0) 
	{
		tempData = tempData >> 1;
		index = index + 1;
	}
	int predictionSize = myEDN->getPredictionSize(index);
	int predictionVal = 1 << predictionSize;
	int dataVal = 1 << index;
	int currState = myFSM->getInitialState();
	int currParity = myEDN->genPrediction(currState);

	if (showStates)
	{
		cout << currState << "," << currParity << endl;
	}

	int totalErrorsDetectedGlobal = 0;;
	int totalErrorsGlobal = 0;

	for (int i = 0; i < inputs.size(); i++)
	{
		int input = inputs.at(i);
		int nextState = myFSM->getNextState(currState, input);
		int nextParity = myEDN->genPrediction(nextState, randNumber);

		if (doErrorAnalysis)
		{
			//TODO: error analysis
			int numErrorsDetected = 0;
			int totalErrors = (predictionVal * dataVal) - 1;
			for (int i = 0; i < dataVal; i++)
			{
				for (int j = 0; j < predictionVal; j++)
				{
					if (i == 0 && j == 0) continue;
					numErrorsDetected += myEDN->doErrorCheck(nextState ^ i, nextParity ^ j);
				}
			}
			totalErrorsDetectedGlobal += numErrorsDetected;
			totalErrorsGlobal += totalErrors;
		}

		currState = nextState;
		currParity = nextParity;
		if (showStates)
		{
			cout << currState << "," << currParity << endl;
		}
	}

	cout << "Detected " << totalErrorsDetectedGlobal << " errors out of " << totalErrorsGlobal << " possible errors." << endl;
}

void FSMSimulator::genVerilog(string filename)
{
	ofstream myfile;
	myfile.open(filename);


	myfile.close();
}

//Getters and setters
void FSMSimulator::setFSM(FiniteStateMachine * FSM)
{
	myFSM = FSM;
}

FiniteStateMachine * FSMSimulator::getFSM()
{
	return myFSM;
}

void FSMSimulator::setEDN(ErrorDetectionNetwork * EDN)
{
	myEDN = EDN;
}

ErrorDetectionNetwork * FSMSimulator::getEDN()
{
	return myEDN;
}