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
	bool showStates = true;
	int currState = myFSM->getInitialState();

	if (showStates)
	{
		cout << currState << endl;
	}

	for (int i = 0; i < inputs.size(); i++)
	{
		int input = inputs.at(i);
		int nextState = myFSM->getNextState(currState, input);


		currState = nextState;
		if (showStates)
		{
			cout << currState << endl;
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