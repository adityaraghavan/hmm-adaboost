#ifndef HMM_H
#define HMM_H

#include <vector>
#include <string>
#include <iostream>

class hmm {
 public:
	std::vector<std::vector<double>> alpha;
	std::vector<std::vector<double>> beta;
	std::vector<std::vector<double>> gamma;
	std::vector<std::vector<std::vector<double>>> digamma;
	std::vector<double> scale;
	std::vector<int> obsv_symb;

	hmm();
	hmm(int /*num_states*/, int /*num_obsv_seq*/);
	~hmm();

	void setNumStates(int /*num_states*/);
	int getNumStates();
	void setNumObsvSeq(int /*num_obsv_seq*/);
	int getNumObsvSeq();
	void setInitDist(const std::vector<double> & /*init_dist*/);
	void setStateTrans(const std::vector<std::vector<double>> & /*state_transition*/);
	void setObsvProbab(const std::vector<std::vector<double>> & /*obsv_probab*/);

	void printStateTransition();
	void printObsvProbab();
	void printInitDist();
	void printAlpha();
	void printBeta();
	void printGamma();

	template<typename T>
	friend std::ostream& operator<<(std::ostream& out, const std::vector<T>&);

	template<typename T>
	friend std::ostream& operator<<(std::ostream& out, const std::vector<std::vector<T>>&);
	
	void ForwardAlgorithm(const std::vector<int> &);
	void BackwardAlgorithm(const std::vector<int> &);
	void CalculateGammas(const std::vector<int> &);
	void Restimate(const std::vector<int> &);

 private:
	std::vector<std::vector<double>> state_trasition;
	std::vector<std::vector<double>> obsv_probab;
	std::vector<double> init_dist;
	int num_states, num_obsv_seq; 
};

#endif