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
	int irreduciblePolynomials[32];

public:
	ErrorDetectionNetwork();
	ErrorDetectionNetwork(ErrorDetectionNetworkType);
	~ErrorDetectionNetwork();

	int genPrediction(int data, int randNumber);
	int getPredictionSize(int);
	void genVerilog(string);
	bool doErrorCheck(int, int);

	//Getters and Setters
	void setType(ErrorDetectionNetworkType);

};