#include "FiniteStateMachine.h"

using namespace std;

enum ErrorDetectionNetworkType
{
	None,
	Hamming2,
	Hamming2and1,
	Nonlinear,
	Multilinear
};

class ErrorDetectionNetwork
{
private:
	ErrorDetectionNetworkType myEDNT;
	FiniteStateMachine * myFSM;

public:
	ErrorDetectionNetwork();
	ErrorDetectionNetwork(ErrorDetectionNetworkType, FiniteStateMachine *);
	~ErrorDetectionNetwork();

	
	void genVerilog(string);
	void simulateFSM(vector<int>, bool);

	//Getters and setters
	void setFSM(FiniteStateMachine *);
	FiniteStateMachine * getFSM();

};