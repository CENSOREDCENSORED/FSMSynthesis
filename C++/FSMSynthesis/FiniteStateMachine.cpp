#include "FiniteStateMachine.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

FiniteStateMachine::FiniteStateMachine(int numStates, int numInputs, int numOutputs, int initialState)
{
	myNumStates = numStates;
	myNumInputs = numInputs;
	myNumOutputs = numOutputs;
	myInitialState = initialState < myNumStates ? initialState > -1 ? initialState : 0 : 0;
	myNextStateArr = new int[myNumStates * myNumInputs];
	myOutputArr = new int[myNumStates * myNumInputs];
	myStateNames = new int[myNumStates];

	for (int i = 0; i < myNumStates; i++)
	{
		myStateNames[i] = i;
	}
}

FiniteStateMachine::FiniteStateMachine()
{
	FiniteStateMachine(DEFAULTSTATES, DEFAULTINPUTS, DEFAULTOUTPUTS, DEFAULTINITIALSTATE);
}

FiniteStateMachine::~FiniteStateMachine()
{
	delete myNextStateArr;
}

void FiniteStateMachine::setStateNames(EncodingHeuristic heuristic)
{
	//TODO: Implement this
}

void FiniteStateMachine::genRandomFSM(int seed)
{
	srand (seed);

	for (int i = 0; i < myNumStates; i++)
	{
		for (int j = 0; j < myNumInputs; j++)
		{
			myNextStateArr[i * myNumInputs + j] = rand() % myNumStates;
			myOutputArr[i * myNumInputs + j] = rand() % myNumStates;
		}
	}
}

void FiniteStateMachine::printFSM()
{
	cout << "Next State Logic" << endl;
	for (int i = -1; i < myNumStates; i++)
	{
		for (int j = -1; j < myNumInputs; j++)
		{
			if (j > -1)
			{
				if (i == -1)
				{
					cout << j << "\t";
				}
				else 
				{
					cout << myNextStateArr[i * myNumInputs + j];
					if (j < myNumInputs - 1) cout << "\t";
				}
			}
			else 
			{
				if (i > -1) cout << i << ":" << "\t";
				else cout << "\t";
			}
		}
		cout << endl;
	}
	cout << endl;
	cout << "Output Logic" << endl;
	for (int i = -1; i < myNumStates; i++)
	{
		for (int j = -1; j < myNumInputs; j++)
		{
			if (j > -1)
			{
				if (i == -1)
				{
					cout << j << "\t";
				}
				else 
				{
					cout << myOutputArr[i * myNumInputs + j];
					if (j < myNumInputs - 1) cout << "\t";
				}
			}
			else 
			{
				if (i > -1) cout << i << ":" << "\t";
				else cout << "\t";
			}
		}
		cout << endl;
	}
}

void FiniteStateMachine::genVerilog(string filename)
{
	ofstream myfile;
	myfile.open(filename);

	//Module declaration
	myfile << "module FSM(" << endl;
	myfile << "\tinput clock," << endl;
	myfile << "\tinput reset," << endl;
	myfile << endl;
	myfile << "\tinput ";
	
	if (myNumInputs > 2)
	{
		int numInputs = myNumInputs - 1;
		int index = -1;
		while (numInputs != 0) 
		{
			numInputs = numInputs >> 1;
			index = index + 1;
		}
		myfile << "[" << index << ":0] ";
	}

	myfile << "in," << endl;
	myfile << "\toutput logic ";

	int outputIndex = 1;

	if (myNumOutputs > 2)
	{
		int numOutputs = myNumOutputs - 1;
		int index = -1;
		while (numOutputs != 0) 
		{
			numOutputs = numOutputs >> 1;
			index = index + 1;
		}
		outputIndex = index + 1;
		myfile << "[" << index << ":0] ";
	}

	myfile << "out" << endl;

	myfile << ");" << endl;
	myfile << endl;

	//State declaration
	myfile << "logic ";

	if (myNumStates > 2)
	{
		int numStates = myNumStates - 1;
		int index = -1;
		while (numStates != 0) 
		{
			numStates = numStates >> 1;
			index = index + 1;
		}
		myfile << "[" << index << ":0] ";
	}

	myfile << "state;" << endl;

	//synchronous always block declaration
	myfile << endl;
	myfile << "always @(posedge clock or posedge reset)" << endl;
	myfile << "begin" << endl;
	myfile << "\tif (reset) state <= " << myStateNames[myInitialState] << ";" << endl;
	myfile << "\telse" << endl;
	myfile << "\tbegin" << endl;

	for (int i = 0; i < myNumStates; i++)
	{
		for (int j = 0; j < myNumInputs; j++)
		{
			myfile << "\t\tif (state == " << myStateNames[i] << " && in == " << j << ")" << endl;
			myfile << "\t\tbegin" << endl;
			myfile << "\t\t\tstate <= " << myStateNames[myNextStateArr[i * myNumInputs + j]]  << ";" << endl;
			myfile << "\t\tend" << endl;
		}
	}
	myfile << "\tend" << endl;

	myfile << "end" << endl;

	//asynchronous always block declaration
	myfile << endl;
	myfile << "always_comb" << endl;
	myfile << "begin" << endl;
	myfile << "\tout <= " << outputIndex << "'b";
	for (int i = 0; i < outputIndex; i++) myfile << "x";
	myfile << ";" << endl;
	for (int i = 0; i < myNumStates; i++)
	{
		for (int j = 0; j < myNumInputs; j++)
		{
			myfile << "\tif (state == " << myStateNames[i] << " && in == " << j << ")" << endl;
			myfile << "\tbegin" << endl;
			myfile << "\t\tout <= " << myOutputArr[i * myNumInputs + j]  << ";" << endl;
			myfile << "\tend" << endl;
		}
	}
	myfile << "end" << endl;


	//endmodule declaration
	myfile << endl;
	myfile << "endmodule" << endl;
	myfile.close();
}