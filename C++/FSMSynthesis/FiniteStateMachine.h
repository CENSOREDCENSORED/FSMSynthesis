#include <vector>

using namespace std;

#define DEFAULTSTATES 2
#define DEFAULTINPUTS 2
#define DEFAULTOUTPUTS 2
#define DEFAULTINITIALSTATE 0

enum EncodingHeuristic
{
	PrioritizedAdjacency,
	HotOne,
	MinimumHammingDistance,
	Hamming2,
	Hamming2and1
};

class FiniteStateMachine
{
private:
	bool genOutputLogic;
	int myNumInputs;
	int myNumStates;
	int myNumOutputs;
	int myInitialState;
	int * myNextStateArr;
	int * myOutputArr;
	int * myStateNames;

	void elimUnreachableStates();
	int decodeState(int);
	void setHamming2();

public:
	FiniteStateMachine();
	FiniteStateMachine(int, int, int, int);
	~FiniteStateMachine();

	void setStateNames(EncodingHeuristic);
	void minimizeFSM();
	void genRandomFSM();
	void printFSM();
	void genVerilog(string);

	//Getters and setters
	int getNumStates();
	int getInitialState();
	int getNextState(int, int);
	void setOutputLogic(bool);
};