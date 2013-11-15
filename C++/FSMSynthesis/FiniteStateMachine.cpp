#include "FiniteStateMachine.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <queue>


FiniteStateMachine::FiniteStateMachine(int numStates, int numInputs, int numOutputs, int initialState)
{
	genOutputLogic = false;

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

	myInitialState = myStateNames[myInitialState];
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

void FiniteStateMachine::minimizeFSM()
{
	elimUnreachableStates();
	//TODO: Implement this
}

int FiniteStateMachine::decodeState(int stateName)
{
	for (int i = 0; i < myNumStates; i++)
	{
		if (myStateNames[i] == stateName) return i;
	}
	return -1;
}

void FiniteStateMachine::elimUnreachableStates()
{
	queue<int> queue;
	queue.push(decodeState(myInitialState));
	int * stateVisited = new int[myNumStates];
	for (int i = 0; i < myNumStates; i++)
	{
		stateVisited[i] = 0;
	}

	int newNumStates = 0;
	while(!queue.empty())
	{
		int state = queue.front();
		stateVisited[state] = 1;
		newNumStates = newNumStates + 1;
		queue.pop();
		int * FSMRow = &(myNextStateArr[state * myNumInputs]);
		
		for (int i = 0; i < myNumInputs; i++)
		{
			int nextState = FSMRow[i];
			if (stateVisited[nextState] == 0){
				queue.push(nextState); 
				stateVisited[nextState] = 1;
			}
		}
	}

	int * stateRemapped = new int[myNumStates];
	int offset = 0;
	for (int i = 0; i < myNumStates; i++)
	{
		if (stateVisited[i] != 0) stateRemapped[i] = i - offset;
		else offset = offset + 1;
	}

	int * newNextState = new int[newNumStates * myNumInputs];
	int * newOutputLogic = new int[newNumStates * myNumInputs];
	int * newStateNames = new int[newNumStates];

	offset = 0;
	for (int i = 0; i < myNumStates; i++)
	{
		if (stateVisited[i] != 0)
		{
			
			newStateNames[i - offset] = stateRemapped[i];

			for (int j = 0; j < myNumInputs; j++)
			{
				newNextState[(i - offset) * myNumInputs + j] = stateRemapped[myNextStateArr[i*myNumInputs + j]];
				newOutputLogic[(i - offset) * myNumInputs + j] = myOutputArr[i*myNumInputs + j];
			}
		}
		else offset++;
	}


	delete myNextStateArr;
	delete myOutputArr;
	myNextStateArr = newNextState;
	myOutputArr = newOutputLogic;

	myNumStates = newNumStates;
	
	myInitialState = newStateNames[stateRemapped[decodeState(myInitialState)]];

	delete myStateNames;
	myStateNames = newStateNames;

	delete stateRemapped;
	delete stateVisited;

}

void FiniteStateMachine::genRandomFSM(int seed)
{
	srand (seed);

	for (int i = 0; i < myNumStates; i++)
	{
		for (int j = 0; j < myNumInputs; j++)
		{
			myNextStateArr[i * myNumInputs + j] = rand() % myNumStates;
			myOutputArr[i * myNumInputs + j] = rand() % myNumOutputs;
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

	
	int outputIndex = 1;
	if (genOutputLogic)
	{
		myfile << "\toutput reg ";


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

		myfile << "out," << endl;
	}

	//Currstate declaration
	myfile << "\toutput reg ";
	int stateIndex = 1;

	if (myNumStates > 2)
	{
		int numStates = myNumStates - 1;
		int index = -1;
		while (numStates != 0) 
		{
			numStates = numStates >> 1;
			index = index + 1;
		}
		stateIndex = index + 1;
		myfile << "[" << index << ":0] ";
	}

	myfile << "currstate," << endl;
	
	//Currstate declaration
	myfile << "\toutput reg ";
	
	myfile << "[" << stateIndex - 1 << ":0] ";

	myfile << "nextstate" << endl;

	myfile << ");" << endl;
	myfile << endl;

	

	//synchronous always block declaration
	myfile << endl;
	myfile << "always @(posedge clock or posedge reset)" << endl;
	myfile << "begin" << endl;
	myfile << "\tif (reset) currstate <= " << myStateNames[myInitialState] << ";" << endl;
	myfile << "\telse currstate <= nextstate;" << endl;

	myfile << "end" << endl;

	//asynchronous always block declaration
	myfile << endl;
	myfile << "always@(*)" << endl;
	myfile << "begin" << endl;
	if (genOutputLogic)
	{
		myfile << "\tout <= " << outputIndex << "'b";
		for (int i = 0; i < outputIndex; i++) myfile << "x";
		myfile << ";" << endl;
	}
	myfile << "\tnextstate <= " << stateIndex << "'b";
	for (int i = 0; i < stateIndex; i++) myfile << "x";
	myfile << ";" << endl;
	for (int i = 0; i < myNumStates; i++)
	{
		for (int j = 0; j < myNumInputs; j++)
		{
			myfile << "\tif (currstate == " << myStateNames[i] << " && in == " << j << ")" << endl;
			myfile << "\tbegin" << endl;
			myfile << "\t\tnextstate <= " << myStateNames[myNextStateArr[i * myNumInputs + j]]  << ";" << endl;
			if (genOutputLogic)
			{
				myfile << "\t\tout <= " << myOutputArr[i * myNumInputs + j]  << ";" << endl;
			}
			myfile << "\tend" << endl;
		}
	}
	myfile << "end" << endl;


	//endmodule declaration
	myfile << endl;
	myfile << "endmodule" << endl;
	myfile.close();
}

//Getters and Setters
int FiniteStateMachine::getNumStates()
{
	return myNumStates;
}

int FiniteStateMachine::getInitialState()
{
	return myInitialState;
}

int FiniteStateMachine::getNextState(int state, int input)
{
	state = decodeState(state);
	if (state == -1) return -1;
	if (state < myNumStates && input < myNumInputs)
	{
		return myStateNames[myNextStateArr[state * myNumInputs + input]];
	}
	else return -1;
}

void FiniteStateMachine::setOutputLogic(bool val)
{
	genOutputLogic = val;
}