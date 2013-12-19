#include <string>

using namespace std;


class Wire
{
private:
	unsigned char myVal;
	unsigned char myNewVal;
	string myName;
	bool isOutput;
	bool isInput;
	double prob0;
	double prob1;

public:
	Wire();
	~Wire();

	void setVal(unsigned char);
	unsigned char getVal();
	void advanceVal();
	unsigned char getNewVal();

	double getProb0();
	double getProb1();
	void setProb0(double);
	void setProb1(double);

	void setName(string );
	string getName();
	bool getIsOutput();
	void setIsOutput(bool);
	bool getIsInput();
	void setIsInput(bool);
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
	void propagateProbs();

	Wire* getOut();
	Wire* getIn1();
	Wire* getIn2();
};