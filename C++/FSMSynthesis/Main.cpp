#include "ErrorCorrection.h"
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
	//booleans
	bool seeOutput = true;

	//ErrorCorrection * ec = new ErrorCorrection();
	//cout << ec->genHammingCodeParity(1) << endl;

	FiniteStateMachine * FSM = new FiniteStateMachine(10, 4, 4, 3);
	//FSM->setOutputLogic(true);
	FSM->genRandomFSM(100);
	FSM->printFSM();
	FSM->minimizeFSM();
	FSM->printFSM();
	FSM->genVerilog("../../Verilog/FSM.v");

	ErrorDetectionNetwork * EDN = new ErrorDetectionNetwork(None);
	FSMSimulator * FSMSim = new FSMSimulator(FSM, EDN);

	vector<int> inputs;
	inputs.push_back(1);
	inputs.push_back(3);
	FSMSim->simulateFSM(inputs, true);

	delete FSMSim;
	delete EDN;
	delete FSM;
	if (seeOutput) while (true){}
}