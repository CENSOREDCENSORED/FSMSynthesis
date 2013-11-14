


class ErrorCorrection
{
private:

public:
	ErrorCorrection();
	~ErrorCorrection();

	unsigned int GFMult(unsigned int , unsigned int , unsigned int , unsigned int );
	unsigned int inverse(unsigned int , unsigned int, unsigned int );
	unsigned int genHammingCodeParity(unsigned int);
	unsigned int genNonLinearHammingCodeParity(unsigned int , unsigned int , unsigned int );
	unsigned int nonLinearize(unsigned int , unsigned int , unsigned int );
};