#pragma once
#include <vector>
#include <string>

class Adaboost
{

public:
	~Adaboost();
	Adaboost(int, int, int);

	int noOfModels;
	int malwareFilesCount;
	int benignFilesCount;

	void boostingAlgorithm();

	std::vector<int> malwareDataClassfn;
	std::vector<int> benignDataClassfn;

	std::vector<double> classifierWts;
};