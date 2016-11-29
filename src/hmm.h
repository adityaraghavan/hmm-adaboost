#pragma once
#include <vector>

class hmm
{
public:
	std::vector<float> alpha;
	std::vector<float> beta;
	std::vector<float> c;
	hmm(int, int, int);
	~hmm();

private:
	std::vector<std::vector<float>> A;
	std::vector<std::vector<float>> B;
	std::vector<std::vector<float>> PI;
	int n, m;
};
