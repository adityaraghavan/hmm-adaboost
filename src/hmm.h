#ifndef HMM_H
#define HMM_H

#include <vector>

class hmm {
 public:
	std::vector<float> alpha;
	std::vector<float> beta;
	std::vector<float> scaling;
	std::vector<int> obsv_symb;

	hmm(int /*num_states*/, int /*num_obsv_seq*/);
	void SetNumStates(int /*num_states*/);
	int GetNumStates();
	void SetNumObsvSeq(int /*num_obsv_seq*/);
	int GetNumObsvSeq();
	void PrintStateTransition();
	void PrintObsvProbab();
	void PrintInitDist();
	void PrintTwoDimVector(const std::vector<std::vector<float>> &);
	~hmm();

 private:
	std::vector<std::vector<float>> state_trasition;
	std::vector<std::vector<float>> obsv_probab;
	std::vector<std::vector<float>> init_dist;
	int num_states, num_obsv_seq; 
};

#endif