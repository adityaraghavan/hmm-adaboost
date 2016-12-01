#include "hmm.h"
#include "ObservationSequences.h"
#include "Adaboost.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "boost\progress.hpp"

using namespace std;

void main()
{
	string malwareName;
	cout << "Enter Malware Family Name - ";
	cin >> malwareName;

	ObservationSequences obSeq(malwareName);
	obSeq.getFileList();
	obSeq.getFileStream();

	/*int noOfModels;
	cout << "Enter Number of HMM models to be created - ";
	cin >> noOfModels;
*/
	//Adaboost boost(noOfModels, obSeq.malwareData.size(), obSeq.benignData.size());
	//boost.boostingAlgorithm();
}