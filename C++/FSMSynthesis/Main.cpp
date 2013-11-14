#include "ErrorCorrection.h"
#include "FiniteStateMachine.h"
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

	FiniteStateMachine * FSM = new FiniteStateMachine(4, 4, 4, 0);
	FSM->genRandomFSM(100);
	FSM->printFSM();
	FSM->genVerilog("../../Verilog/FSM.sv");

	if (seeOutput) while (true){}
}