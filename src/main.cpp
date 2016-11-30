#include "hmm.h"
#include <iostream>
#include <vector>

using namespace std;

void main()
{
	vector<vector <float>> pstate = { {0.7, 0.3}, {0.4, 0.6} };
	vector<vector <float>> pobsv = { { 0.1, 0.4, 0.5 },{ 0.7, 0.2, 0.1 } };
	vector<float> init = { 0.0, 1.0 };
	vector<int> obsv = { 1, 0, 2 };

	//hmm obj();
	
}