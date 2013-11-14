#include "ErrorCorrection.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

void main()
{
	ErrorCorrection * ec = new ErrorCorrection();
	cout << ec->genHammingCodeParity(1) << endl;


}