#include "DigitalLogic.h"

//Wire
Wire::Wire()
{
	myVal = 0;
	myNewVal = 0;
	isOutput = true;
	isInput = true;
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