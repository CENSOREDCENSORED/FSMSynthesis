#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <windows.h>
#include "Circuit.h"
#define _CRTDBG_MAP_ALLOC

using namespace std;

int measurePower(int var, int modVal)
{
	return (rand() % modVal) + modVal;
}

//Prints if the test has actually triggered the trojan only once
bool printDifferenceDetected = true;

//Prints the partition groups only once
bool printPartitionGroups = true;

double doSimulation(int numiter, bool doTimings, int timingIteration, int noiseMargin, Circuit * goldenCircuit, 
				  Circuit * trojanCircuit, 
#ifdef DETAILEDRESULTS
				  int numInitialResults, int * highestPowResults, unsigned long long ** highestPowScanChains,  
#endif // DETAILEDRESULTS

				  bool printPower, 
				  bool partition, int index, 
				  int numPartitionGroups, int numPartitions, int partitionSize, int currPartitionGroup)
{
	int secs1;
	int secs2;
	int totalSum = 0;
	for (int i = 0; i < numiter; i++)
	{
		if (doTimings && i % timingIteration == 0)
		{
			secs1 = time(NULL);
		}

		//Pseudorandom normal distrubtion
		int noiseTrojan = 0; 
		int noiseGolden = 0;

		if (noiseMargin > 0)
		{
			for (int i = 0; i < 16; i++)
			{

				noiseTrojan += (rand() % (2*noiseMargin+1)) - noiseMargin;
				noiseGolden += (rand() % (2*noiseMargin+1)) - noiseMargin;
			} 
		}

		noiseTrojan /= 16;
		noiseGolden /= 16;

		int goldenPowerNoNoise = goldenCircuit->genNextPowerMeasurement(index, partition,
			numPartitionGroups, numPartitions, partitionSize, currPartitionGroup,printPartitionGroups);
		int goldenPower = goldenPowerNoNoise + noiseGolden;
		goldenPower = goldenPower < 0 ? 0 : goldenPower;
		printPartitionGroups = false;
		int trojanPowerNoNoise = trojanCircuit->genNextPowerMeasurement(index, partition,
			numPartitionGroups, numPartitions, partitionSize, currPartitionGroup,printPartitionGroups);
		int trojanPower = trojanPowerNoNoise + noiseTrojan;
		trojanPower = trojanPower < 0 ? 0 : trojanPower;
		if ((goldenPowerNoNoise - trojanPowerNoNoise) != 0 && printDifferenceDetected)
		{
			cout << "difference!" << endl;
			printDifferenceDetected = false;
		}

		int difference = trojanPower - goldenPower;
		totalSum += difference;

		int minIndex = -1;

#ifdef DETAILEDRESULTS
		for (int numResultsIndex = 0; numResultsIndex < numInitialResults; numResultsIndex++)
		{
			int powResult = highestPowResults[numResultsIndex];
			if (difference > powResult)
			{
				minIndex = numResultsIndex;
			}
		}

		if (minIndex > -1)
		{
			highestPowResults[minIndex] = difference;
			if (highestPowScanChains[minIndex] != 0)
			{
				delete highestPowScanChains[minIndex];
			}
			highestPowScanChains[minIndex] = goldenCircuit->getScanChainVals();

		}  
#endif // DETAILEDRESULTS


		//bool trojanDetected = (trojanPower != goldenPower);
		//string printString = "";//trojanDetected ? "Trojan Detected" : "Trojan Not Detected";
		if (printPower)
		{
			cout << goldenPower << "," << trojanPower //<< "," << printString 
			<< endl;
		}
			
		if (doTimings && i % timingIteration == timingIteration-1)
		{
			secs2 = time(NULL);
			cout << "Generated in " << secs2 - secs1 << " seconds" << endl;
		}
	}

	return totalSum * 1.0 / numiter;
}

void main()
{
	int seed = 100;
	srand (seed);

	int numStates = 10;
	int numInputs = 4;
	int numOutputs = 4;
	int initialState = 3;

	int numTransitions = 100;

	//booleans
	bool testRandNumGen = false;
	bool testFSM = false;
	bool testPartitioning = false;
	bool testPowAnalysis = true;
	bool seeOutput = true;

	if (testRandNumGen)
	{
		RandNumGenerator * rng = new RandNumGenerator();
		rng->setPolynomial(rng->getPolynomial(5)>>1);
		rng->seedRandNumGen(1);
		long long var = 1;
		for (int i = 0; i < 10; i++)
		{
			cout << var << endl;
			var = rng->genRandNum();
		}

		delete rng;
	}

	//ErrorCorrection * ec = new ErrorCorrection();
	//cout << ec->genHammingCodeParity(1) << endl;

	if (testFSM)
	{
		FiniteStateMachine * FSM = new FiniteStateMachine(numStates, numInputs, numOutputs, initialState);
		//FSM->setOutputLogic(true);
		FSM->genRandomFSM();
		//FSM->printFSM();
		FSM->minimizeFSM();
		//FSM->printFSM();
		//FSM->genVerilog("../../Verilog/FSM.v");

		//ErrorDetectionNetwork * EDN = new ErrorDetectionNetwork(Hamming2);
		//ErrorDetectionNetwork * EDN = new ErrorDetectionNetwork(Linear);
		ErrorDetectionNetwork * EDN = new ErrorDetectionNetwork(Nonlinear);
		FSMSimulator * FSMSim = new FSMSimulator(FSM, EDN);

		vector<int> inputs;
		for (int i = 0; i < numTransitions; i++)
		{
			inputs.push_back(rand() % numInputs);
		}
		FSMSim->simulateFSM(inputs, true, "ErrorCountNonLinearHamming.csv");

		EDN->setType(LinearHamming);

		FSMSim->simulateFSM(inputs, true, "ErrorCountLinear.csv");

		EDN->setType(LinearParity);

		FSMSim->simulateFSM(inputs, true, "ErrorCountLinearParity.csv");

		EDN->setType(Hamming2);
		FSM->setStateNames(Hamming2Encoding);
		FSMSim->simulateFSM(inputs, true, "ErrorCountHamming2.csv");


		delete FSMSim;
		delete EDN;
		delete FSM;
	}

	if (testPartitioning)
	{
		int B = 5;
		int S = 5;
		int numScanCellsInPartition = 5;
		for (int c = 0; c < B; c++)
		{
			for (int b = 0; b < S; b++)
			{
				for (int i = 0; i < numScanCellsInPartition; i++)
				{
					int partition = i*B + ((b + c*i) % B);
					cout << partition;
					if (i < (numScanCellsInPartition - 1)) cout << ",";
				}
				cout << "|";
			}
			cout << endl;
		}
	}

	if (testPowAnalysis)
	{
		bool doTimings = false;
		int timingIteration = 100;
		bool printPower = false;
		bool printMaxScanChains = false;
		bool outputToFile = true;

		//0x52b24a62
		//1387418449
		//1387423210
		//1387428972
		seed = time(NULL);

		ofstream powOutputfile;
		ofstream runnableOutputFile;
		if (outputToFile)
		{
			string fileName = "";
			ostringstream convert;
			convert << seed;
			fileName.append(convert.str());
			fileName.append("PowOutput.csv");

			powOutputfile.open(fileName);

			string seedFileName = "";
			seedFileName.append(convert.str());
			seedFileName.append("Runnable.py");

			runnableOutputFile.open(seedFileName);

			runnableOutputFile << "from interpretData import interpretData" << endl << endl;
			runnableOutputFile << "interpretData(" << seed << ")" << endl;
			runnableOutputFile.close();
		}
		
		cout << "Seed is " << seed << endl;

		int baseGates = 1000;
		int offsetGates = 1000;
		int offsetOutputs = 1000;
		int numiter = 20000;
		
		int noiseMargin = 100;

		int numScan = 25;
		int sizeScan = 10;

		int numInitialResults = 100;
		
#ifdef DETAILEDRESULTS
		int * highestPowResults = new int[numInitialResults];
		unsigned long long ** highestPowScanChains = new unsigned long long * [numInitialResults];

		for (int i = 0; i < numInitialResults; i++)
		{
			highestPowResults[i] = 0;
			highestPowScanChains[i] = 0;
		}
#endif

		Circuit * goldenCircuit = new Circuit(numScan, sizeScan, false);
		Circuit * trojanCircuit = new Circuit(numScan, sizeScan, true);

		
		int secs1 = time(NULL);
		goldenCircuit->genRandomCircuit(seed,baseGates,offsetGates,offsetOutputs);
		int secs2 = time(NULL);
		cout << "Generated in " << secs2 - secs1 << " seconds. Golden Circuit ";
		goldenCircuit->printNumGates();

		secs1 = time(NULL);
		trojanCircuit->genRandomCircuit(seed,baseGates,offsetGates,offsetOutputs);
		secs2 = time(NULL);
		cout << "Generated in " << secs2 - secs1 << " seconds. Trojan Circuit ";
		trojanCircuit->printNumGates();

		unsigned long long * scanChainSeeds = new unsigned long long[numScan];
		for (unsigned long long i = 0; i < numScan; i++)
		{
			scanChainSeeds[i] = i+1;
		}

		//trojanCircuit->printGates();

		//SIMULATION PORTION
		bool partition = true;
		int numPartitionGroups = 5;
		int numPartitions = 5;
		int partitionSize = 5;
		int totalRuns = 50;

		double * powResults = new double[numPartitionGroups * numPartitions];

		for (int currPartitionGroup = 0; currPartitionGroup < numPartitionGroups; currPartitionGroup++)
		{
			for (int currPartition = 0; currPartition < numPartitions; currPartition++)
			{
				int B = numPartitionGroups;
				int S = numPartitions;
				int numScanChainsInPartition = partitionSize;
				int c = currPartitionGroup;
				int b = currPartition;
				for (int i = 0; i < numScanChainsInPartition; i++)
				{
					int partition = i*B + ((b + c*i) % B);

					if (outputToFile) powOutputfile << partition;
					if (outputToFile && (i < (numScanChainsInPartition - 1))) powOutputfile << ",";
				}
				if (outputToFile) powOutputfile << "\t";
			}
			
			if (outputToFile) powOutputfile << endl;
		}

		//TODO: Output all this crap into a .csv file
		for (int currRun = 0; currRun < totalRuns; currRun++)
		{
			cout << "Curr run is " << currRun << endl;
			if (outputToFile)
			{
				powOutputfile << "Run " << currRun << endl;
			}
			bool hasPrintedSeedsToFile = false; 

			for (int currPartitionGroup = 0; currPartitionGroup < numPartitionGroups; currPartitionGroup++)
			{
				cout << "Curr partition group is " << currPartitionGroup << endl;
				for (int currPartition = 0; currPartition < numPartitions; currPartition++)
				{
					cout << "Curr partition is " << currPartition << endl;

					srand(time(NULL));
					secs1 = time(NULL);
					goldenCircuit->seedScanChains(scanChainSeeds, numScan, currRun);
					secs2 = time(NULL);
					//cout << "Golden circuit seeded in " << secs2 - secs1 << " seconds." << endl;
		
					secs1 = time(NULL);
					trojanCircuit->seedScanChains(scanChainSeeds, numScan, currRun);
					secs2 = time(NULL);
					//cout << "Trojan circuit seeded in " << secs2 - secs1 << " seconds." << endl;

					if (outputToFile && !hasPrintedSeedsToFile)
					{
						unsigned long long * scVals = goldenCircuit->getScanChainVals();
						for (int scanIndex = 0; scanIndex < numScan; scanIndex++)
						{
							powOutputfile << std::hex << scVals[scanIndex] << ",";
						}
						powOutputfile << endl << std::dec;
						hasPrintedSeedsToFile = true;
						delete scVals;
					}
			
					//TODO: Make these not global variables
					printPartitionGroups = true;
					printDifferenceDetected = true;
					double average = doSimulation(numiter, doTimings, timingIteration, noiseMargin, goldenCircuit, 
							  trojanCircuit, 
		#ifdef DETAILEDRESULTS
							  numInitialResults, highestPowResults, highestPowScanChains,  
		#endif // DETAILEDRESULTS
							  printPower,
							  partition, currPartition,numPartitionGroups,numPartitions,partitionSize,currPartitionGroup);
		
					cout << average << endl;
					cout << endl;

					powResults[currPartition + (currPartitionGroup*numPartitions)] = average;
				}
				cout << "--------------------" << endl;
			}

			if (outputToFile)
			{
				for (int printIndexI = 0; printIndexI < numPartitionGroups; printIndexI++)
				{
					for (int printIndexJ = 0; printIndexJ < numPartitions; printIndexJ++)
					{
						powOutputfile << powResults[printIndexJ + (printIndexI*numPartitions)] << "\t";
					}
					powOutputfile << endl;
				}

				powOutputfile << endl;
			}
		}
		
#ifdef DETAILEDRESULTS
		int highestPowIndex = -1;
		int highestPowVal = 0;
		for (int i = 0; i < numInitialResults; i++)
		{
			if (highestPowResults[i] > highestPowVal) 
			{
				highestPowVal = highestPowResults[i];
				highestPowIndex = i;
			}
		}

		cout << highestPowVal << endl;

		double maxAverage = 0;
		int maxScanChain = -1;
		
		for (int i = 0; i < goldenCircuit->getNumScanChains(); i++)
		{
			//int * nextHighestPowResults = new int[numInitialResults];
			//unsigned long long ** nextHighestPowScanChains = new unsigned long long * [numInitialResults];
			if (highestPowScanChains[highestPowIndex] != 0)
			{
				goldenCircuit->seedScanChains(highestPowScanChains[highestPowIndex], goldenCircuit->getNumScanChains());
				trojanCircuit->seedScanChains(highestPowScanChains[highestPowIndex], goldenCircuit->getNumScanChains());
				average = doSimulation(numiter, doTimings, timingIteration, noiseMargin, goldenCircuit, 
						  trojanCircuit, numInitialResults, highestPowResults, highestPowScanChains, printPower,
						  i);
				cout << average << endl;

				if (average > maxAverage) 
				{
					maxScanChain = i;
					maxAverage = average;
				}

				highestPowIndex = -1;
				highestPowVal = 0;
				for (int i = 0; i < numInitialResults; i++)
				{
					if (highestPowResults[i] > highestPowVal) 
					{
						highestPowVal = highestPowResults[i];
						highestPowIndex = i;
					}
				}

				//cout << highestPowVal << endl;
			}
		}

		cout << maxScanChain << "," << maxAverage << endl;

		double oldMaxAverage = maxAverage;

		for (int i = 0; i < numInitialResults; i++)
		{
			unsigned long long * scanChainSeeds = highestPowScanChains[i];
			goldenCircuit->seedScanChains(scanChainSeeds, goldenCircuit->getNumScanChains());
			trojanCircuit->seedScanChains(scanChainSeeds, goldenCircuit->getNumScanChains());
			
			average = doSimulation(numiter, doTimings, timingIteration, noiseMargin, goldenCircuit, 
						trojanCircuit, numInitialResults, highestPowResults, highestPowScanChains, printPower,
						maxScanChain);
			cout << average << endl;

			if (average > maxAverage) 
				{
					//maxScanChain = i;
					maxAverage = average;
				}

				highestPowIndex = -1;
				highestPowVal = 0;
				for (int i = 0; i < numInitialResults; i++)
				{
					if (highestPowResults[i] > highestPowVal) 
					{
						highestPowVal = highestPowResults[i];
						highestPowIndex = i;
					}
				}

				//cout << highestPowVal << endl;
		}

		cout << oldMaxAverage << "," << maxAverage << endl;

		//DEBUG STATEMENT

		if (printMaxScanChains)
		{
			for (int i = 0; i < numInitialResults; i++)
			{

				cout << highestPowResults[i];

				if (highestPowScanChains[i] != 0) 
				{
					for (int j = 0; j < goldenCircuit->getNumScanChains(); j++)
					{
						cout << "," << highestPowScanChains[i][j];
					}
				}

				cout << endl;
			} 
		}


		delete highestPowResults;

		for (int i = 0; i < numInitialResults; i++)
		{
			if (highestPowScanChains[i] != 0) delete highestPowScanChains[i];
		}
		delete highestPowScanChains;
#endif // DETAILEDRESULTS

		if (outputToFile)
		{
			powOutputfile.close();
		}

		delete scanChainSeeds;
		delete powResults;

		delete goldenCircuit;
		delete trojanCircuit;
	}

	_CrtDumpMemoryLeaks();

	if (seeOutput)
	{
		cout << "Done" << endl;
		while (true){Sleep(1000);}
	}
}