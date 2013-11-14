#include "FiniteStateMachine.h"

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


	//Getters and setters
	void setFSM(FiniteStateMachine *);
	FiniteStateMachine * getFSM();
};