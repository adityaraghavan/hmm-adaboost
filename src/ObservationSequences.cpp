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

ObservationSequences::ObservationSequences(std::string folderName) 
{

	noOfFiles = 0;
	noOfTrainingFiles = 0;
	noOfScoringFiles = 0;
	this->parentFolderPath = "D:/Aditya/CS_266/Project/Dataset/";
	this->malwareFamilyName = folderName;
}

void ObservationSequences::getFileList() {

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
	this->noOfTrainingFiles = this->noOfFiles * 0.6 + 1;

	this->getTrainingData();
	this->getScoringData();
}

void ObservationSequences::getTrainingData() 
{
	int lastIndex = this->noOfTrainingFiles;
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
			}
			else
			{
				this->distinctOpCodesList.push_back(line);
				int newOpCodeIndex = this->distinctOpCodesList.size() - 1;

				this->trainingData.push_back(newOpCodeIndex);
			}
		}
	}
}

void ObservationSequences::getScoringData()
{
	int firstIndex = this->noOfTrainingFiles;
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

			if (opCodeIndex != endIndex)
			{
				tempFileStream.push_back(opCodeIndex);
			}
			else
			{
				this->distinctOpCodesList.push_back(line);
				int newOpCodeIndex = this->distinctOpCodesList.size() - 1;

				tempFileStream.push_back(newOpCodeIndex);
			}
		}
		this->scoringData.push_back(tempFileStream);
	}

	addBenignData();
	this->noOfScoringFiles = this->scoringData.size();
}

void ObservationSequences::addBenignData() {

	string folderName = this->parentFolderPath + "/Benign/";
	bst::path p(folderName);
	for (auto i = bst::directory_iterator(p); i != bst::directory_iterator(); i++)
	{
		if (!bst::is_directory(i->path()))
		{
			string fullFileName = folderName + i->path().filename().string();
			getBenignData(fullFileName);
		}
	}
}

void ObservationSequences::getBenignData(string fullFileName) {

	vector<int> tempFileStream;

	ifstream tempReadFile;
	tempReadFile.open(fullFileName);

	string line;
	while (getline(tempReadFile, line))
	{
		int opCodeIndex = find(this->distinctOpCodesList.begin(), this->distinctOpCodesList.end(), line) - this->distinctOpCodesList.begin();
		int endIndex = this->distinctOpCodesList.size();

		if (opCodeIndex != endIndex)
		{
			tempFileStream.push_back(opCodeIndex);
		}
		else
		{
			this->distinctOpCodesList.push_back(line);
			int newOpCodeIndex = this->distinctOpCodesList.size() - 1;

			tempFileStream.push_back(newOpCodeIndex);
		}
	}
	this->scoringData.push_back(tempFileStream);
}