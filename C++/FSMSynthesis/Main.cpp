#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include "Circuit.h"

using namespace std;

int measurePower(int var, int modVal)
{
	return (rand() % modVal) + modVal;
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
	bool testPowAnalysis = true;
	bool seeOutput = true;

	if (testRandNumGen)
	{
		RandNumGenerator * rng = new RandNumGenerator(0x19 >> 1);
		rng->seedRandNumGen(1);
		long long var = 1;
		for (int i = 0; i < 100; i++)
		{
			cout << var << endl;
			var = rng->genRandNum();
		}
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

	if (testPowAnalysis)
	{
		Circuit * circuit = new Circuit();

		circuit->genRandomCircuit(100);
		circuit->printGates();

		delete circuit;

		/*ScanChain * sc = new ScanChain(10);
		sc->initScanChain(1);
		for (int i = 0; i < 10; i++)
		{
			sc->printScanChain();
			sc->incrementScanChain();
			cout << endl;
		}


		delete sc;*/

		/*seed = 120;
		srand(seed);
		int step = 2;

		for (int thingy = 0; thingy < 2; thingy++)
		{
			//srand(seed);

			//Parameters
			int trojanLength = 10;
			int sideChannelPowerOffset = 1;
			int noiseMargin = 8000;
			int powerMargin = 5000;
			int numiter = 20000;
			int numScanChains = 1;

			bool hasTrojan = thingy & 1;//rand() % 2;

			ofstream myfile;
			string filename = "";
			if (!hasTrojan) filename.append("No");
			filename.append("TrojanResults");

			ostringstream convert;
			convert << thingy/2;
			filename.append(convert.str());

			filename.append(".csv");
			myfile.open(filename);

			int * trojanSeq = new int[trojanLength];
			int * trojanIndex = new int[trojanLength];
			int * trojanScan = new int[trojanLength];
			int * scanChainIndex = new int[trojanLength];
			for (int i = 0; i < trojanLength; i++)
			{
				trojanSeq[i] = rand() % 2;
				trojanIndex[i] = rand() % trojanLength;
				trojanScan[i] = rand() % numScanChains;
				scanChainIndex[i] = rand() % 4;
				cout << scanChainIndex[i] << "," << trojanSeq[i] << "," << trojanIndex[i] << endl;
			}
			cout << "--------------" << endl;
			RandNumGenerator * sc1 = new RandNumGenerator(0x481 >> 1);
			RandNumGenerator * sc2 = new RandNumGenerator(0x481 >> 1);
			RandNumGenerator * sc3 = new RandNumGenerator(0x481 >> 1);
			RandNumGenerator * sc4 = new RandNumGenerator(0x481 >> 1);
			sc1->seedRandNumGen(1);
			sc2->seedRandNumGen(2);
			sc3->seedRandNumGen(3);
			sc4->seedRandNumGen(4);
			int count = 0;

			srand(time(NULL));

			for (int i = 0; i < numiter; i++)
			{
				//Pseudorandom normal distrubtion
				int noiseTrojan = 0; 
				int noiseGolden = 0;
				for (int i = 0; i < 16; i++)
				{
					noiseTrojan += (rand() % (2*noiseMargin+1)) - noiseMargin;
					noiseGolden += (rand() % (2*noiseMargin+1)) - noiseMargin;
				}

				noiseTrojan /= 16;
				noiseGolden /= 16;

				//input test vector via lfsr
				int var1 = sc1->genRandNum();
				int var2 = sc2->genRandNum();
				int var3 = sc3->genRandNum();
				int var4 = sc4->genRandNum();

				if (step == 2)
				{
					var1 = 920;
					var2 = 945;
					var3 = 41;
					//var4 = 995;
				}

				int powerMeasurement = measurePower(var1, powerMargin);
				int powerMeasurementGolden = powerMeasurement;
				powerMeasurement += noiseTrojan;
				powerMeasurementGolden += noiseGolden;

				bool trojIndex = false; 
				int scIndex = scanChainIndex[count];
				int var = var1;
				switch(scIndex)
				{
				case 0:
					var = var1; 
					break;
				case 1:
					var = var2;
					break;
				case 2:
					var = var3;
					break;
				case 3:
					var = var4;
					break;
				default:
					var = var1;
					break;
				}

				if (hasTrojan && (((var >> trojanIndex[count]) & 1) == trojanSeq[count]))
				{
					count++;
					powerMeasurement += sideChannelPowerOffset;
					trojIndex = true;
				}
				else 
				{
					count = 0;
				}

				bool trojanPrediction = false;//(powerMeasurement - powerMeasurementGolden) > (sideChannelPowerOffset + noiseMargin/10);

				//cout << var << "," << powerMeasurement << "," << powerMeasurementGolden << "," << trojIndex 
				//	<< "," << trojanPrediction << endl;

				myfile << "," << powerMeasurement << "," << powerMeasurementGolden << "," << trojIndex 
					<< "," << trojanPrediction << ",";

				myfile << "=A" << i+1 << "*B" << i+1 << ",";
				myfile << "=A" << i+1 << "*C" << i+1 << ",";
				myfile << "=B" << i+1 << "-C" << i+1 << ",";
				myfile << var1 << "," << var2 << "," << var3 << "," << var4 << ",";
				myfile << endl;
			}

			myfile << "=AVERAGE(A1:A" << numiter << "),=AVERAGE(B1:B" << numiter << "),=AVERAGE(C1:C" << numiter << "),,,=AVERAGE(F1:F" << numiter << ")," 
				<< "=AVERAGE(G1:G" << numiter << "),";
			
			myfile << endl;

			myfile << ",=(-(A" << numiter+1 << "*B" << numiter+1 << ")+F" << numiter+1 << ")/(STDEV(A1:A" << numiter << ")*STDEV(B1:B" << numiter << ")),";
			myfile << "=(-(A" << numiter+1 << "*C" << numiter+1 << ")+G" << numiter+1 << ")/(STDEV(A1:A" << numiter << ")*STDEV(C1:C" << numiter << ")),";
			myfile << endl;

			delete scanChainIndex;
			delete trojanSeq;
			delete trojanIndex;
			delete trojanScan;

			myfile.close();
		}*/
	}

	if (seeOutput) while (true){}
}