#include "FiniteStateMachine.h"
#include "ErrorCorrection.h"

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
	ErrorCorrection * ec;

public:
	ErrorDetectionNetwork();
	ErrorDetectionNetwork(ErrorDetectionNetworkType);
	~ErrorDetectionNetwork();

	int genPrediction(int);
	int getPredictionSize(int);
	void genVerilog(string);
	bool doErrorCheck(int, int);


};