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
	int index = 0;
	int data = myFSM->getNumStates();
	while (data != 0) 
	{
		data = data >> 1;
		index = index + 1;
	}
	int predictionSize = myEDN->getPredictionSize(index);
	cout << index << ":" << predictionSize << endl;
	int predictionVal = 1 << predictionSize;
	bool showStates = true;
	int currState = myFSM->getInitialState();
	int currParity = myEDN->genPrediction(currState);

	if (showStates)
	{
		cout << currState << "," << currParity << endl;
	}

	for (int i = 0; i < inputs.size(); i++)
	{
		int input = inputs.at(i);
		int nextState = myFSM->getNextState(currState, input);
		int nextParity = myEDN->genPrediction(nextState);

		if (doErrorAnalysis)
		{
			//TODO: error analysis
			
		}

		currState = nextState;
		currParity = nextParity;
		if (showStates)
		{
			cout << currState << "," << currParity << endl;
		}
	}
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