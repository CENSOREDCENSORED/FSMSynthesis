#include "FiniteStateMachine.h"
#include "ErrorCorrection.h"
#include "RandNumGenerator.h"

using namespace std;

enum ErrorDetectionNetworkType
{
	None,
	Hamming2,
	Hamming2and1,
	LinearParity,
	LinearHamming,
	Nonlinear,
	Multilinear
};

class ErrorDetectionNetwork
{
private:
	ErrorDetectionNetworkType myEDNT;
	ErrorCorrection * ec;
	unsigned int irreduciblePolynomials[32];
	RandNumGenerator * rng;

public:
	ErrorDetectionNetwork();
	ErrorDetectionNetwork(ErrorDetectionNetworkType);
	~ErrorDetectionNetwork();

	int genPrediction(int data, int randNumber, unsigned int irrPoly, unsigned int numElems);
	int getPredictionSize(int);
	void genVerilog(string);
	bool doErrorCheck(int data, int check, int randNumber, unsigned int irrPoly, unsigned int numElems);
	bool usesRandom();

	//Getters and Setters
	void setType(ErrorDetectionNetworkType);
	void setRandNumGenPolynomial(unsigned long long polynomial);
	unsigned long long getPolynomial(int degree);
	unsigned long long getRandNumber();
	void seedRNG(unsigned long long seed);
};