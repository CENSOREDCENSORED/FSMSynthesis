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
	MinimumHammingDistance
};

class FiniteStateMachine
{
private:
	int myNumInputs;
	int myNumStates;
	int myNumOutputs;
	int myInitialState;
	int * myNextStateArr;
	int * myOutputArr;
	int * myStateNames;

	void elimUnreachableStates();

public:
	FiniteStateMachine();
	FiniteStateMachine(int, int, int, int);
	~FiniteStateMachine();

	void setStateNames(EncodingHeuristic);
	void minimizeFSM();
	void genRandomFSM(int);
	void printFSM();
	void genVerilog(string);

	//Getters
	int getNumStates();
};