#include "ErrorCorrection.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

ErrorCorrection::ErrorCorrection()
{
}

ErrorCorrection::~ErrorCorrection()
{
}

unsigned int ErrorCorrection::GFMult(unsigned int operand1, unsigned int operand2, 
									 unsigned int irrPoly, unsigned int numElems)
{
	unsigned int result = 0;
	
	//Perform multiply
	for (int i = 0; i < numElems; i++){
		unsigned int mask = operand2 & 1;
		if (mask == 1) result ^= operand1 << i;
		operand2 = operand2 >> 1;
	}

	int MSBIndex = 0;
	for (int i = 0; i < 32; i++){
		if (result >> i != 0) MSBIndex = i;
	}

	//cout << std::hex << result << "," << MSBIndex << endl;
	int numElemsMinusOne = numElems-1;
	int index = 0;
	while (numElemsMinusOne != 0)
	{
		numElemsMinusOne = numElemsMinusOne >> 1;
		index++;
	}

	//Perform mod with irreducible polynomial
	for (int i = MSBIndex - index; i >=0; i--){
		if (result >= (1 << (i + index))){
			result = (irrPoly << i) ^ result;
		}
	}


	return result;
}

unsigned int ErrorCorrection::inverse(unsigned int operand, unsigned int irrPoly, unsigned int numElems)
{
	for (unsigned int i = 0; i < numElems; i++)
	{
		if (GFMult(operand, i, irrPoly, numElems) == 1) return i;
	}

	return operand;
}

unsigned int ErrorCorrection::genSingleBitParity(unsigned int data)
{
	unsigned int result = 0;
	for (int i = 0; i < 32; i++){
		result ^= (data >> i) & 1;
	}

	return result;
}

unsigned int ErrorCorrection::genHammingCodeParity(unsigned int data)
{
	unsigned int offset = 0;
	unsigned int paritybits= 0;
	for (int i = 0; i < 38; i++)
	{
		unsigned int mask = 0x1; 
		if (i == 0 || i == 1 || i == 3 || i == 7 || i == 15 || i == 31)
		{
			offset += 1;
			continue;
		}
		else 
		{
			unsigned int indexOfData = i - offset + 1;
			
			unsigned int dataBit = (data >> (indexOfData-1)) & 0x1;
			
			if (dataBit)
			{
				for (int j = 0; j < 32; j++)
				{
					unsigned int parityLocation = (i+1) & (0x1 << j);
					if (parityLocation){
						//cout << isNonZero << endl;
						unsigned int temp3 = dataBit << j;
						paritybits = paritybits ^ temp3;

						/*ostringstream convert;

						string result = "";
	
						convert << temp1;
						result.append(convert.str());
						cout << result << endl;

						result.clear();*/

					}
				}
			}

		}
	}
	return paritybits;
}

unsigned int ErrorCorrection::nonLinearize(unsigned int operand, unsigned int irrPoly, 
										   unsigned int numElems)
{
	return GFMult(operand, GFMult(operand, operand, irrPoly, numElems), irrPoly, numElems);
}

unsigned int ErrorCorrection::genNonLinearHammingCodeParity(unsigned int data, unsigned int irrPoly, 
															unsigned int numElems)
{
	return nonLinearize(genHammingCodeParity(data), irrPoly, numElems);
}

int ErrorCorrection::getHammingPredictionSize(int dataSize)
{
	if (dataSize < 5) return 3;
	if (dataSize < 12) return 4;
	if (dataSize < 27) return 5;
	return 6;
}