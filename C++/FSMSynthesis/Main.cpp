#include "ErrorCorrection.h"
#include "ErrorDetectionNetwork.h"
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
	bool seeOutput = false;

	//ErrorCorrection * ec = new ErrorCorrection();
	//cout << ec->genHammingCodeParity(1) << endl;

	FiniteStateMachine * FSM = new FiniteStateMachine(100, 4, 4, 3);
	FSM->genRandomFSM(100);
	//FSM->printFSM();
	FSM->minimizeFSM();
	//FSM->printFSM();
	FSM->genVerilog("../../Verilog/FSM.v");

	delete FSM;
	if (seeOutput) while (true){}
}