#include "FSMSimulator.h"


class Wire
{
private:
	unsigned char myVal;

public:
	Wire();
	~Wire();

	void setVal(unsigned char);
	unsigned char getVal();
};

class NandGate
{
private:
	Wire * myIn1;
	Wire * myIn2; 
	Wire * myOut;
public:
	NandGate(Wire*, Wire*, Wire*);
	~NandGate();
	bool genOutput();

	Wire* getOut();
	Wire* getIn1();
	Wire* getIn2();
};

class ScanChain
{
private:
	int mySize;
	Wire ** myFlopArr;
	RandNumGenerator * myRandNumGenerator;

	void loadScanChain(int);

public:
	ScanChain();
	ScanChain(int size);
	~ScanChain();

	void initScanChain();
	void initScanChain(unsigned long long);
	void incrementScanChain();
	void printScanChain();

	int getSize();
	unsigned char getFlopVal(int);
	Wire * getFlopNet(int);
};