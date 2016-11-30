#ifndef HMM_H
#define HMM_H

#include <vector>
#include <string>
#include <iostream>

class hmm {
 public:
	std::vector<std::vector<float>> alpha;
	std::vector<std::vector<float>> beta;
	std::vector<float> scaling;
	std::vector<int> obsv_symb;

	hmm();
	hmm(int /*num_states*/, int /*num_obsv_seq*/);
	~hmm();

	void setNumStates(int /*num_states*/);
	int getNumStates();
	void setNumObsvSeq(int /*num_obsv_seq*/);
	int getNumObsvSeq();
	void printStateTransition();
	void printObsvProbab();
	void printInitDist();

	template<typename T>
	friend std::ostream& operator<<(std::ostream& out, const std::vector<T>&);

	template<typename T>
	friend std::ostream& operator<<(std::ostream& out, const std::vector<std::vector<T>>&);
	
	void ForwardAlgorithm(const std::vector<int> &);
	void BackwardAlgorithm(const std::vector<int> &);

 private:
	std::vector<std::vector<float>> state_trasition;
	std::vector<std::vector<float>> obsv_probab;
	std::vector<float> init_dist;
	int num_states, num_obsv_seq; 
};

#endif