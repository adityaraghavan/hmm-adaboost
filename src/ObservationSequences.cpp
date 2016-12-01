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
	this->getScoringData();
	this->addBenignData();
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
		this->malwareData.push_back(tempFileStream);
	}
}

void ObservationSequences::addBenignData() 
{
	string folderName = this->parentFolderPath + "/benign/";
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

void ObservationSequences::getBenignData(string fullFileName) 
{
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
	this->benignData.push_back(tempFileStream);
}