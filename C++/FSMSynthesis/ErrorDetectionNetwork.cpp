#include "ErrorDetectionNetwork.h"
#include "ErrorCorrection.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>


ErrorDetectionNetwork::ErrorDetectionNetwork()
{
	ErrorDetectionNetwork(None, nullptr);
}

ErrorDetectionNetwork::ErrorDetectionNetwork(ErrorDetectionNetworkType type, FiniteStateMachine * FSM)
{
	myEDNT = type;
	myFSM = FSM;


}

ErrorDetectionNetwork::~ErrorDetectionNetwork()
{

}


void ErrorDetectionNetwork::genVerilog(string filename)
{
	ofstream myfile;
	myfile.open(filename);


	myfile.close();
}

void ErrorDetectionNetwork::simulateFSM(vector<int> inputs, bool doErrorAnalysis)
{
	bool showStates = false;
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

//Getters and setters
void ErrorDetectionNetwork::setFSM(FiniteStateMachine * FSM)
{
	myFSM = FSM;
}

FiniteStateMachine * ErrorDetectionNetwork::getFSM()
{
	return myFSM;
}