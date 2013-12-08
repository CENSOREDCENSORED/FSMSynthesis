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

public:
	Wire();
	~Wire();

	void setVal(unsigned char);
	unsigned char getVal();
	void advanceVal();
	unsigned char getNewVal();

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

	Wire* getOut();
	Wire* getIn1();
	Wire* getIn2();
};