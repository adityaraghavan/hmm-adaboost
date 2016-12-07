#include "ObservationSequences.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "boost\filesystem.hpp"

using namespace std;
namespace bst = boost::filesystem;

ObservationSequences::~ObservationSequences() 
{
}

ObservationSequences::ObservationSequences(std::string folderName, int dist_opcodes)
{
	noOfFiles = 0;
	this->dist_opcodes = dist_opcodes;
	this->parentFolderPath = "./input/dataset/";
	this->malwareFamilyName = folderName;
}

void ObservationSequences::getFileList() 
{
	string folderName = this->parentFolderPath + this->malwareFamilyName + "/";
	bst::path p(folderName);
	for (auto i = bst::directory_iterator(p); i != bst::directory_iterator(); i++)
	{
		if (!bst::is_directory(i->path()))
		{	
			string fullFileName = folderName + i->path().filename().string();
			this->fileNameList.push_back(fullFileName);
		}
	}
}

void ObservationSequences::getFileStream() 
{
	this->noOfFiles = this->fileNameList.size();

	this->getTrainingData();
	this->sortOpCodes();
	this->getScoringData();
	this->getBenignData();
}

void ObservationSequences::getTrainingData()
{
	int lastIndex = this->noOfFiles * 0.6 + 1;
	for (int index = 0; index < lastIndex; index++)
	{
		vector<int> tempFileStream;

		string tempFileName = this->fileNameList.at(index);
		ifstream tempReadFile;
		tempReadFile.open(tempFileName);

		string line;
		while (getline(tempReadFile, line))
		{
			int opCodeIndex = find(this->distinctOpCodesList.begin(), this->distinctOpCodesList.end(), line) - this->distinctOpCodesList.begin();
			int endIndex = this->distinctOpCodesList.size();

			if (opCodeIndex != endIndex)
			{
				this->trainingData.push_back(opCodeIndex);
				this->distinctOpCodesCount[opCodeIndex]++;
			}
			else
			{
				this->distinctOpCodesList.push_back(line);
				int newOpCodeIndex = this->distinctOpCodesList.size() - 1;

				this->trainingData.push_back(newOpCodeIndex);
				this->distinctOpCodesCount.push_back(1);
			}
		}
	}
}

void ObservationSequences::sortOpCodes()
{

	sortedOpCodesIndexes.resize(this->distinctOpCodesCount.size(), 0);
	std::size_t n(0);
	std::generate(std::begin(sortedOpCodesIndexes), std::end(sortedOpCodesIndexes), [&] { return n++; });

	std::sort(std::begin(sortedOpCodesIndexes),
		std::end(sortedOpCodesIndexes),
		[&](int i1, int i2) { return this->distinctOpCodesCount[i1] > this->distinctOpCodesCount[i2]; });

	updateFileStream(this->trainingData);
	//updateScoringData();
}

void ObservationSequences::updateFileStream(vector<int> &fileStream)
{
	int streamsize = fileStream.size();
	for (int index = 0; index < streamsize; index++)
	{
		int opCodeIndex = fileStream[index];
		int tempNewIndex = find(this->sortedOpCodesIndexes.begin(), this->sortedOpCodesIndexes.end(), opCodeIndex) - this->sortedOpCodesIndexes.begin();
		if (tempNewIndex < dist_opcodes)
		{
			fileStream[index] = tempNewIndex;
		}
		else
		{
			fileStream[index] = dist_opcodes;
		}
	}
}

void ObservationSequences::getScoringData()
{
	int firstIndex = this->noOfFiles * 0.6 + 1;
	for (int index = firstIndex; index < this->noOfFiles; index++)
	{
		vector<int> tempFileStream;

		string tempFileName = this->fileNameList.at(index);
		ifstream tempReadFile;
		tempReadFile.open(tempFileName);

		string line;
		while (getline(tempReadFile, line))
		{
			int opCodeIndex = find(this->distinctOpCodesList.begin(), this->distinctOpCodesList.end(), line) - this->distinctOpCodesList.begin();
			int endIndex = this->distinctOpCodesList.size();

			if (opCodeIndex < dist_opcodes)
			{
				tempFileStream.push_back(opCodeIndex);
				this->distinctOpCodesCount[opCodeIndex]++;
			}
			else
			{
				tempFileStream.push_back(dist_opcodes);
				this->distinctOpCodesCount[dist_opcodes]++;
			}
		}
		this->malwareData.push_back(tempFileStream);
	}
}

void ObservationSequences::getBenignData()
{
	string folderName = this->parentFolderPath + "/benign/";
	bst::path p(folderName);
	for (auto i = bst::directory_iterator(p); i != bst::directory_iterator(); i++)
	{
		if (!bst::is_directory(i->path()))
		{
			string fullFileName = folderName + i->path().filename().string();
			addBenignData(fullFileName);
		}
	}
}

void ObservationSequences::addBenignData(string fullFileName)
{
	vector<int> tempFileStream;

	ifstream tempReadFile;
	tempReadFile.open(fullFileName);

	string line;
	while (getline(tempReadFile, line))
	{
		int opCodeIndex = find(this->distinctOpCodesList.begin(), this->distinctOpCodesList.end(), line) - this->distinctOpCodesList.begin();
		int endIndex = this->distinctOpCodesList.size();

		if (opCodeIndex < dist_opcodes)
		{
			tempFileStream.push_back(opCodeIndex);
			this->distinctOpCodesCount[opCodeIndex]++;
		}
		else {
			tempFileStream.push_back(dist_opcodes);
			this->distinctOpCodesCount[dist_opcodes]++;
		}
		//else
		//{
		//	this->distinctOpCodesList.push_back(line);
		//	int newOpCodeIndex = this->distinctOpCodesList.size() - 1;

		//	tempFileStream.push_back(newOpCodeIndex);
		//	this->distinctOpCodesCount.push_back(1);
		//}
	}
	this->benignData.push_back(tempFileStream);
}

//void ObservationSequences::updateScoringData()
//{
//	int malwareFileCount = malwareData.size();
//	for (int fileindex = 0; fileindex < malwareFileCount; fileindex++)
//	{
//		updateFileStream(malwareData[fileindex]);
//	}
//
//	int benignFileCount = benignData.size();
//	for (int fileindex = 0; fileindex < benignFileCount; fileindex++)
//	{
//		updateFileStream(benignData[fileindex]);
//	}
//}