#include "RandNumGenerator.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

RandNumGenerator::RandNumGenerator()
{
	//RandNumGenerator(0);
	initIrreduciblePolynomials();
	myPolynomial = 0;
	myCurrnum = 0;
}

RandNumGenerator::RandNumGenerator(unsigned long long polynomial)
{
	myPolynomial = polynomial;
	myCurrnum = 0;
	initIrreduciblePolynomials();
}

void RandNumGenerator::initIrreduciblePolynomials()
{
	
	irreduciblePolynomials = new unsigned long long[32];

	irreduciblePolynomials[0] = 0x1;
	irreduciblePolynomials[1] = 0x3;
	irreduciblePolynomials[2] = 0x7;
	irreduciblePolynomials[3] = 0xD;
	irreduciblePolynomials[4] = 0x19;
	irreduciblePolynomials[5] = 0x29;
	irreduciblePolynomials[6] = 0x61;
	irreduciblePolynomials[7] = 0xC1;
	irreduciblePolynomials[8] = 0x171;
	irreduciblePolynomials[9] = 0x221;
	irreduciblePolynomials[10] = 0x481;
}

RandNumGenerator::~RandNumGenerator()
{
	delete irreduciblePolynomials;
}

unsigned long long RandNumGenerator::genRandNum()
{
	//TODO: implement this
	unsigned long long nextNum = myCurrnum;

	nextNum = (nextNum >> 1) ^ (-((long long) nextNum & 1) & myPolynomial);

	myCurrnum = nextNum;

	return nextNum;
}

void RandNumGenerator::genVerilog(string filename)
{
	ofstream myfile;
	myfile.open(filename);


	myfile.close();
}

void RandNumGenerator::setPolynomial(unsigned long long polynomial)
{
	myPolynomial = polynomial;
}

void RandNumGenerator::seedRandNumGen(unsigned long long seed)
{
	myCurrnum = seed;
}

unsigned long long RandNumGenerator::getPolynomial(int degree)
{
	return irreduciblePolynomials[degree];
}

unsigned long long RandNumGenerator::getCurrNum()
{
	return myCurrnum;
}