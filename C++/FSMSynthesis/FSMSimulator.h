#include "ErrorDetectionNetwork.h"

using namespace std;

class FSMSimulator
{
private:
	FiniteStateMachine * myFSM;
	ErrorDetectionNetwork * myEDN;

public:
	FSMSimulator();
	FSMSimulator(FiniteStateMachine *, ErrorDetectionNetwork *);
	~FSMSimulator();

	void genVerilog(string);
	void simulateFSM(vector<int>, bool, string);
	
	
	//Getters and setters
	void setFSM(FiniteStateMachine *);
	FiniteStateMachine * getFSM();
	void setEDN(ErrorDetectionNetwork *);
	ErrorDetectionNetwork * getEDN();
};