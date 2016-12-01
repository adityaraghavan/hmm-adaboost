#include "hmm.h"
#include "ObservationSequences.h"
#include "Adaboost.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "boost\progress.hpp"

using namespace std;

vector<vector<double>> GetRandomVector(int, int);

void main()
{
	int num_models = 1;
	int iter = 100;
	vector<double> row;
	string malwarename;

	/*cout << "Enter Number of HMM models to be created - ";
	cin >> num_models;*/

	for (int i = 0; i< (num_models * 2); i++)
	{
		double u = (double)rand() / (RAND_MAX + 1) + 40 + (rand() % 20);
		row.push_back(u/100);
	}
	
	cout << "enter malware family name - ";
	cin >> malwarename;

	ObservationSequences obseq(malwarename);
	obseq.getFileList();
	obseq.getFileStream();

	Hmm model[1];
	
	vector<double> temp;
	vector< vector<double>> temp2;

	for (int i = 0; i < num_models; i++)
	{	
		
		// set n & m
		model[i].setNumStates(2);
		model[i].setNumObsvSeq(obseq.distinctOpCodesList.size());

		//initialize pi
		temp.push_back(row[i]);
		temp.push_back(1 - row[i]);
		model[i].setInitDist(temp);
		temp.clear();
		//model[i].printInitDist();

		//initialize A
		temp.push_back(row[row.size() - 1 - (i * 2)]);
		temp.push_back(1 - temp[0]);
		temp2.push_back(temp);
		temp.clear();
		temp.push_back(row[row.size() - 2 - (i * 2)]);
		temp.push_back(1 - temp[0]);
		temp2.push_back(temp);
		temp.clear();
		model[i].setStateTrans(temp2);
		temp2.clear();
		//model[i].printStateTransition();

		//Initialize B
		model[i].setObsvProbab(GetRandomVector(rand(), model[i].getNumObsvSeq()));
		model[i].printObsvProbab();

		boost::progress_display progress(iter);
		for (int it = 0; it < iter; it++)
		{
			model[it].ForwardAlgorithm(obseq.trainingData);
			model[it].BackwardAlgorithm(obseq.trainingData);
			model[it].CalculateGammas(obseq.trainingData);
			model[it].Restimate(obseq.trainingData);
			model[it].printInitDist();
			model[it].printStateTransition();
			++progress;
		}

		ofstream out("out.txt");
		streambuf *coutbuf = std::cout.rdbuf(); //save old buf
		cout.rdbuf(out.rdbuf());

		cout << "\nTesting malware samples: \n";
		// Test Malware
		int test_size = obseq.malwareData.size();
		vector<double> test_malscore;
		for (int j = 0; j < test_size; j++)
		{
			test_malscore.push_back(model[i].Score(obseq.malwareData.at(j)));
			cout << test_malscore.back() << "\n";
		}

		cout << "\nTesting benign samples: \n";
		//Test Benign
		test_size = obseq.benignData.size();
		vector<double> test_benscore;
		for (int j = 0; j < test_size; j++)
		{
			test_benscore.push_back(model[i].Score(obseq.benignData.at(j)));
			cout << test_benscore.back() << "\n";
		}


	}

	system("pause");
	//Adaboost boost(noOfModels, obSeq.malwareData.size(), obSeq.benignData.size());
	//boost.boostingAlgorithm();
}

vector<vector<double>> GetRandomVector(int rand, int num)
{
	vector<double> row;
	vector<vector<double>> result;
	double distribSum = 0;

	for (int i = 1; i < num+1; i++)
	{
		double temp = (rand + (double)i / 10);
		row.push_back(temp);
		distribSum += temp;
	}

	for (int i = 0; i < num; i++)
	{
		row[i] = row[i] / distribSum;
	}

	result.push_back(row);
	reverse(row.begin(), row.end());
	result.push_back(row);

	return result;
}