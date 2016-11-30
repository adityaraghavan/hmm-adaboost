#include "hmm.h"
#include <iostream>
#include <vector>

using namespace std;

void main()
{
	vector<vector <double>> pstate = { {0.7, 0.3}, {0.4, 0.6} };
	vector<vector <double>> pobsv = { { 0.1, 0.4, 0.5 },{ 0.7, 0.2, 0.1 } };
	vector<double> init = { 0.0, 1.0 };
	vector<int> obsv = { 1, 0, 2 };

	hmm obj(2, 3);
	obj.setInitDist(init);
	obj.setStateTrans(pstate);
	obj.setObsvProbab(pobsv);
	obj.printInitDist();
	obj.printObsvProbab();
	obj.ForwardAlgorithm(obsv);
	obj.printAlpha();
	
}