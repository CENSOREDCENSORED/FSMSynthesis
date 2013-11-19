class ErrorCorrection
{
private:

public:
	ErrorCorrection();
	~ErrorCorrection();

	unsigned int GFMult(unsigned int , unsigned int , unsigned int , unsigned int );
	unsigned int inverse(unsigned int , unsigned int, unsigned int );
	unsigned int genSingleBitParity(unsigned int data);
	unsigned int genHammingCodeParity(unsigned int data);
	unsigned int genNonLinearHammingCodeParity(unsigned int , unsigned int , unsigned int );
	unsigned int nonLinearize(unsigned int , unsigned int , unsigned int );
	int getHammingPredictionSize(int);
};