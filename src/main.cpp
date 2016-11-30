#include "hmm.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "boost\progress.hpp"

using namespace std;

void main()
{
	vector<vector <double>> pstate = { {0.47468, 0.52532}, {0.51656, 0.48344} };
	vector<vector <double>> pobsv(2, vector<double>(27,0));
	vector<double> init = { 0.51316, 0.48684 };
	vector<int> obsv;

	ifstream input("input/brown.txt");

	char c;
	int temp, count = 0;
	while (input.get(c) && count < 50000)
	{
		if (islower(c))
		{
			temp = c;
			temp -= 97;
		}
		else if (isspace(c))
		{
			temp = 26;
		}
		obsv.push_back(temp);
		count++;
	}
	
	vector<double> row;
	double distribSum = 0;
	for (int i = 1; i < 28; i++)
	{
		double temp = (17 + (double)i/10);
		row.push_back(temp);
		distribSum += temp;
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 27; j++)
		{
			pobsv[i][j] = row[j] / distribSum;
		}
	}

	hmm obj(2, 27);
	obj.setInitDist(init);
	obj.setStateTrans(pstate);
	obj.setObsvProbab(pobsv);
	
	boost::progress_display progress(100);

	for (int i = 0; i < 3; i++)
	{
		obj.ForwardAlgorithm(obsv);
		obj.BackwardAlgorithm(obsv);
		obj.CalculateGammas(obsv);
		obj.Restimate(obsv);
		obj.printStateTransition();
		++progress;
	}
	
	obj.printInitDist();
	obj.printObsvProbab();
}