#include "DigitalLogic.h"

//Wire
Wire::Wire()
{
	myVal = 0;
	myNewVal = 0;
	isOutput = true;
	isInput = true;
	prob0 = 0.5;
	prob1 = 0.5;
}

Wire::~Wire()
{
}

void Wire::setVal(unsigned char newVal)
{
	myNewVal = newVal;
}

void Wire::advanceVal()
{
	myVal = myNewVal;
}

unsigned char Wire::getVal()
{
	return myVal;
}

double Wire::getProb0()
{
	return prob0;
}

double Wire::getProb1()
{
	return prob1;
}

void Wire::setProb0(double newProb)
{
	prob0 = newProb;
}

void Wire::setProb1(double newProb)
{
	prob1 = newProb;
}

void Wire::setName(string name)
{
	myName = name;
}

string Wire::getName()
{
	return myName;
}

bool Wire::getIsOutput()
{
	return isOutput;
}

void Wire::setIsOutput(bool val)
{
	isOutput = val;
}

bool Wire::getIsInput()
{
	return isInput;
}

void Wire::setIsInput(bool val)
{
	isInput = val;
}

unsigned char Wire::getNewVal()
{
	return myNewVal;
}

//NandGate
NandGate::NandGate(Wire * in1, Wire * in2, Wire * out)
{
	myIn1 = in1;
	myIn2 = in2;
	myOut = out;
}

NandGate::~NandGate()
{
}

bool NandGate::genOutput()
{
	unsigned char origOut = myOut->getVal();
	myOut->setVal((1 ^ (myIn1->getVal() & myIn2->getVal())) & 1);
	return origOut != myOut->getNewVal();
}

void NandGate::propagateProbs()
{
	double newProb0 = myIn1->getProb1() * myIn2->getProb1();

	if (myIn1 == myIn2) 
	{
		newProb0 = myIn1->getProb0();
	}

	double newProb1 = 1 - newProb0;

	myOut->setProb0(newProb0);
	myOut->setProb1(newProb1);
}

Wire * NandGate::getOut()
{
	return myOut;
}

Wire * NandGate::getIn1()
{
	return myIn1;
}

Wire * NandGate::getIn2()
{
	return myIn2;
}