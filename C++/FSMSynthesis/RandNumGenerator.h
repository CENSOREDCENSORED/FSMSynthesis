#include "string"

using namespace std;

class RandNumGenerator
{
private:
	unsigned int myCurrnum;
	unsigned int myPolynomial;

public:
	RandNumGenerator();
	RandNumGenerator(unsigned int polynomial);
	~RandNumGenerator();
	
	unsigned int genRandNum();
	void genVerilog(string filename);

	//Getters and Setters
	void setPolynomial(unsigned int polynomial);
	void seedRandNumGen(unsigned int seed);
};