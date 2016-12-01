#include "Adaboost.h"
#include "ObservationSequences.h"
#include <iostream>
#include <vector>

using namespace std;

Adaboost::~Adaboost()
{
}

Adaboost::Adaboost(int noOfModels, int noOfMalwareFiles, int noOfBenignFiles)
{
	this->noOfModels = noOfModels;
	this->malwareFilesCount = noOfMalwareFiles;
	this->benignFilesCount = noOfBenignFiles;

	this->malwareDataClassfn.resize(malwareFilesCount, 0);
	this->benignDataClassfn.resize(benignFilesCount, 0);
}

void Adaboost::boostingAlgorithm() 
{
	////assifierWts.resize(this->noOfModels, (std::vector<double> (this->noOfScoringFiles, 1/this->noOfScoringFiles)));
	//
	//for (int j = 0; j < this->noOfModels; j++) {
	//	double tempSum = 0;
	//	for (int i = 0; i < this->noOfScoringFiles; i++) {
	//		 //assifierWts.at(i) * 
	//	}
	//	//this->errorFactor.at(j) = 2;
	//}

	//for (int index = this->noOfScoringFiles - 1; index > 0; index++) {
	//	//classifierWts.at(index + 1) = classifierWts.at(index) * exp(-alpha * *);
	//}

	//double tempSum = 0;
	//for (int index = 0; index < this->noOfScoringFiles; index++) {
	//	tempSum += classifierWts.at(index);
	//}

	//for (int index = 0; index < this->noOfScoringFiles; index++) {
	//	classifierWts.push_back(classifierWts.at(index) / tempSum);
	//}
}