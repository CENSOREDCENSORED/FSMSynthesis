#include "ErrorDetectionNetwork.h"
#include "ErrorCorrection.h"

ErrorDetectionNetwork::ErrorDetectionNetwork()
{
	ErrorDetectionNetwork(None, nullptr);
}

ErrorDetectionNetwork::ErrorDetectionNetwork(ErrorDetectionNetworkType type, FiniteStateMachine * FSM)
{
	myEDNT = type;
	myFSM = FSM;
}

ErrorDetectionNetwork::~ErrorDetectionNetwork()
{

}



//Getters and setters
void ErrorDetectionNetwork::setFSM(FiniteStateMachine * FSM)
{
	myFSM = FSM;
}

FiniteStateMachine * ErrorDetectionNetwork::getFSM()
{
	return myFSM;
}