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

void FSMSimulator::simulateFSM(vector<int> inputs, bool doErrorAnalysis, string errorAnalysisFile)
{
	bool showStates = true;
	bool usesRandNumGen = myEDN->usesRandom();

	int index = 0;
	int data = myFSM->getNumStates();
	int tempData = data - 1;
	while (tempData != 0) 
	{
		tempData = tempData >> 1;
		index = index + 1;
	}
	int predictionSize = myEDN->getPredictionSize(index);
	int predictionVal = 1 << predictionSize;
	int dataVal = 1 << index;
	//cout << index << "," << predictionSize << endl;
	//cout << dataVal << "," << predictionVal << endl;

	//TODO: generate better seed
	unsigned long long polynomial = myEDN->getPolynomial(index);
	myEDN->seedRNG(1);
	myEDN->setRandNumGenPolynomial(polynomial >> 1);
	polynomial = myEDN->getPolynomial(index);
	unsigned long long randNumber = myEDN->getRandNumber();

	int currState = myFSM->getInitialState();
	long long currParity = myEDN->genPrediction(currState, randNumber, polynomial, dataVal);

	if (showStates)
	{
		cout << currState << "," << currParity << endl;
	}

	int totalErrorsDetectedGlobal = 0;
	int totalErrorsGlobal = 0;

	int totalErrors = (predictionVal * dataVal) - 1;
	int * errorCount = new int[totalErrors+1];
	if (usesRandNumGen)
	{
		delete errorCount;
		totalErrors = (predictionVal * dataVal * predictionVal) - 1;
		errorCount = new int[totalErrors+1];
	}

	for (int i = 0; i < totalErrors+1;i++)
	{
		errorCount[i] = 0;
	}

	for (int i = 0; i < inputs.size(); i++)
	{
		randNumber = myEDN->getRandNumber();
		int input = inputs.at(i);
		int nextState = myFSM->getNextState(currState, input);
		int nextParity = myEDN->genPrediction(nextState, randNumber, polynomial, dataVal);

		if (doErrorAnalysis)
		{
			//TODO: error analysis
			int numErrorsDetected = 0;
			
			for (int i = 0; i < dataVal; i++)
			{
				for (int j = 0; j < predictionVal; j++)
				{
					if (usesRandNumGen) 
					{
						for (int k = 0; k < predictionVal; k++)
						{
							if (i == 0 && j == 0 && k == 0) continue;
							long long errorsDetected = myEDN->doErrorCheck(nextState ^ i, nextParity ^ j, randNumber ^ k, polynomial, dataVal);
							if (errorsDetected)
							{
								unsigned int errorIndex = k | (j << predictionSize) | (i << (predictionSize*2));
								errorCount[errorIndex] = errorCount[errorIndex] + 1;
							}
							numErrorsDetected += errorsDetected;
						}
					}
					else
					{
						if (i == 0 && j == 0) continue;
						long long errorsDetected = myEDN->doErrorCheck(nextState ^ i, nextParity ^ j, randNumber, polynomial, dataVal);
						if (errorsDetected)
						{
							unsigned int errorIndex = j | (i << predictionSize);
							errorCount[errorIndex] = errorCount[errorIndex] + 1;
						}
						numErrorsDetected += errorsDetected;
					}
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

	if (doErrorAnalysis)
	{
		ofstream myfile;
		myfile.open(errorAnalysisFile);

		for (int i = 0; i < totalErrors+1; i++)
		{
			myfile << errorCount[i] << endl;
		}

		myfile << "=STDEV(A" << totalErrors+1 << ":A1)" << endl;
		myfile << endl;


		myfile << totalErrorsDetectedGlobal << "," << totalErrorsGlobal << "," << "=100*" << 
			totalErrorsDetectedGlobal << "/" << totalErrorsGlobal << endl;

		myfile << myFSM->getNumStates() << endl;
		myfile.close();
	}

	delete errorCount;

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