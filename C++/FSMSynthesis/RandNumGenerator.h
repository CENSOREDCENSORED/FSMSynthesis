#include "string"

using namespace std;

//This is just an LFSR =[
class RandNumGenerator
{
private:
	unsigned long long myCurrnum;
	unsigned long long myPolynomial;
	unsigned long long * irreduciblePolynomials;

public:
	RandNumGenerator();
	RandNumGenerator(unsigned long long polynomial);
	~RandNumGenerator();
	void initIrreduciblePolynomials();
	
	unsigned long long genRandNum();
	void genVerilog(string filename);

	//Getters and Setters
	void setPolynomial(unsigned long long polynomial);
	void seedRandNumGen(unsigned long long seed);
	unsigned long long getPolynomial(int);
	unsigned long long getCurrNum();
};