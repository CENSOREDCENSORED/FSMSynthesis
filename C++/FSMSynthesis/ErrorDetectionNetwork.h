#include "FiniteStateMachine.h"

using namespace std;

enum ErrorDetectionNetworkType
{
	None,
	Hamming2,
	Hamming2and1,
	Linear,
	Nonlinear,
	Multilinear
};

class ErrorDetectionNetwork
{
private:
	ErrorDetectionNetworkType myEDNT;

public:
	ErrorDetectionNetwork();
	ErrorDetectionNetwork(ErrorDetectionNetworkType);
	~ErrorDetectionNetwork();

	int genPrediction(int);
	void genVerilog(string);
	bool doErrorCheck(int, int);


};