#include "RandNumGenerator.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

RandNumGenerator::RandNumGenerator()
{
	RandNumGenerator(0);
}

RandNumGenerator::RandNumGenerator(unsigned int polynomial)
{
	myPolynomial = polynomial;
	myCurrnum = 0;
}

RandNumGenerator::~RandNumGenerator()
{
}

unsigned int RandNumGenerator::genRandNum()
{
	//TODO: implement this
	unsigned int nextNum = myCurrnum;

	nextNum = (nextNum >> 1) ^ (-((int) nextNum & 1) & myPolynomial);

	myCurrnum = nextNum;

	return nextNum;
}

void RandNumGenerator::genVerilog(string filename)
{
	ofstream myfile;
	myfile.open(filename);


	myfile.close();
}

void RandNumGenerator::setPolynomial(unsigned int polynomial)
{
	myPolynomial = polynomial;
}

void RandNumGenerator::seedRandNumGen(unsigned int seed)
{
	myCurrnum = seed;
}